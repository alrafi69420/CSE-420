#include "symbol_info.h"



extern ofstream outlog; //for wwting to log file from scope_table class


class scope_table
{
private:
    int bucket_count;
    int unique_id;
    scope_table *parent_scope = NULL;
    vector<list<symbol_info *>> table;

    int hash_function(string name)
    {
        // write your hash function here
        int hash = 0;
        for (char c : name) {
            hash += c;
        }
        return hash % bucket_count;
    }

public:
    scope_table();
    scope_table(int bucket_count, int unique_id, scope_table *parent_scope);
    scope_table *get_parent_scope();
    int get_unique_id();
    symbol_info *lookup_in_scope(symbol_info* symbol);
    bool insert_in_scope(symbol_info* symbol);
    bool delete_from_scope(symbol_info* symbol);
    void print_scope_table(ofstream& outlog);
    ~scope_table();

    // you can add more methods if you need
};

// complete the methods of scope_table class


scope_table::scope_table()                            // Runs when a scope table is created.
{
    this->bucket_count = 10;
    this->unique_id = 1;       //First scope = global scope
    this->table.resize(bucket_count);

    outlog << "New ScopeTable with id " << unique_id << " created" << endl << endl;
}

scope_table::scope_table(int bucket_count, int unique_id, scope_table *parent_scope)        //Used when creating new inner scopes.
{
    this->bucket_count = bucket_count;
    this->unique_id = unique_id;
    this->parent_scope = parent_scope;
    this->table.resize(bucket_count);

    outlog << "New ScopeTable with id " << unique_id << " created" << endl << endl;
}

scope_table* scope_table::get_parent_scope()
{
    return parent_scope;
}

int scope_table::get_unique_id()                //Returns the scope ID.
{
    return unique_id;
}

symbol_info* scope_table::lookup_in_scope(symbol_info* symbol)        //Searches for a symbol inside this scope only.
{
    int index = hash_function(symbol->getname());                    //Find bucket index.

    for(auto &sym : table[index]) {                                   //Go through all symbols stored at this hash index 
        if(sym->getname() == symbol->getname()) {
            return sym;
        }
    }

    return NULL;
}

bool scope_table::insert_in_scope(symbol_info* symbol)
{
    if(lookup_in_scope(symbol) != NULL) {                    //Check duplicate
        return false;
    }

    int index = hash_function(symbol->getname());
    table[index].push_back(symbol);

    return true;
}


bool scope_table::delete_from_scope(symbol_info* symbol)
{
    int index = hash_function(symbol->getname());

    for(auto it = table[index].begin(); it != table[index].end(); ++it) {            // Iterate through all the symbols in that bucket
        if((*it)->getname() == symbol->getname()) {                                    // Checks if current symbol name =  target name
            table[index].erase(it);
            return true;
        }
    }

    return false;
}





void scope_table::print_scope_table(ofstream& outlog)                
{
    outlog << "ScopeTable # "+ to_string(unique_id) << endl;            // Print the ID of the current scope table

    //iterate through the current scope table and print the symbols and all relevant information
    for(int i = 0; i < bucket_count; i++) {
        if(table[i].size() > 0) {
            outlog << i << " --> " << endl;            // Print the bucket index

            for(auto &symbol : table[i]) {

                outlog << "< " << symbol->getname() 
                       << " : " << symbol->get_type() << " > " << endl;

                if(symbol->get_type_id() == 0) {        // If the symbol is var
                    outlog << "Variable" << endl;
                    outlog << "Type: " << symbol->get_data_type() << endl;
                }
                else if(symbol->get_type_id() == 1) {        // If the symbol is arr
                    outlog << "Array" << endl;
                    outlog << "Type: " << symbol->get_data_type() << endl;
                    outlog << "Size: " << symbol->get_array_size() << endl;
                }
                else {                                                    // If the symbol is  func
                    outlog << "Function Definition" << endl;
                    outlog << "Return Type: " << symbol->get_return_type() << endl;

                    vector<pair<string,string>> parameters = symbol->get_parameters();

                    outlog << "Parameters: " << parameters.size() << endl;
                    outlog << "Parameter Details: ";

                    for(int j = 0; j < parameters.size(); j++) {
                        if(j == parameters.size() - 1)
                            outlog << parameters[j].first << " " << parameters[j].second;
                        else
                            outlog << parameters[j].first << " " << parameters[j].second << ", ";
                    }

                    outlog << endl;
                }
            }

            outlog << endl;
        }
    }

    outlog << endl;



}

scope_table::~scope_table()
{
    if(unique_id != 1) {
        outlog << "ScopeTable with ID " << unique_id << " removed" << endl << endl;
    }

    for(auto &bucket : table) {
        for(auto &symbol : bucket) {
            delete symbol;
        }
        bucket.clear();
    }

    table.clear();
}

