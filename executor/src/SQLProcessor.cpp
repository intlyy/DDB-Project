#include "../include/SQLProcessor.h"

SQLProcessor::SQLProcessor(std::string query){
    this->query = query;
    this->is_valid = true;  // the sql is valid by default.
    hsql::SQLParser::parseSQLString(this->query, &this->result);
    if (this->result.isValid()) {
        // hsql::SQLStatement*
        this->stat = this->result.getStatements()[0];
        vector<string> key_name; 
        // hsql::printStatementInfo(this->stat);
        switch (this->stat->type()) {
            // select statement
            case hsql::kStmtSelect:
                this->sql_type = 1;
                // static_cast to hsql::SelectStatement*
                this->select_stat = static_cast<hsql::SelectStatement*>(this->stat);
                // generate select_list
                if(this->select_stat->selectList) {
                    for (const hsql::Expr* expr : *this->select_stat->selectList) {
                        std::string rname;
                        std::string aname;
                        // eg. select * from user;
                        if(expr->type == hsql::kExprStar) {
                            // eg. select * from user;
                            if(this->select_stat->fromTable->type == hsql::kTableName) {
                                rname = lower_string(this->select_stat->fromTable->getName());
                                for(auto col:getTableFromEtcd(rname).cols){
                                    this->select.add_select(rname + std::string(".") + col.name);
                                }
                            }
                            // eg. select * from user,book;
                            else if(this->select_stat->fromTable->type == hsql::kTableCrossProduct) {
                                for(const hsql::TableRef* t : *this->select_stat->fromTable->list){
                                    rname = lower_string(t->getName());
                                    for(auto col:getTableFromEtcd(rname).cols){
                                        this->select.add_select(rname + std::string(".") + col.name);
                                    }
                                }
                            }
                        }
                        // eg. select name,id from user;
                        else if(expr->type == hsql::kExprColumnRef) {
                            if(expr->hasTable()) {
                                // eg. select user.id from user;
                                rname = lower_string(expr->table);
                            } else {
                                // eg. select id from user;
                                if(this->select_stat->fromTable->type == hsql::kTableName) {
                                    rname = lower_string(this->select_stat->fromTable->getName());
                                }
                                // [TODO] Column 'aname' in field list is ambiguous
                            }
                            aname = lower_string(expr->getName());
                            this->select.add_select(lower_string(rname) + std::string(".") + lower_string(aname));
                        }
                    }
                }

                // generate from_list
                if(this->is_valid && this->select_stat->fromTable) {
                    // eg. select * from user;
                    std::string rname;
                    if(this->select_stat->fromTable->type == hsql::kTableName) {
                        rname = lower_string(this->select_stat->fromTable->getName());
                        this->select.add_from(rname);
                    }
                    // eg. select * from user,book where book.user_id=user.id;
                    else if(this->select_stat->fromTable->type == hsql::kTableCrossProduct) {
                        for(const hsql::TableRef* t : *this->select_stat->fromTable->list){
                            rname = lower_string(t->getName());
                            this->select.add_from(rname);
                        }
                    }
                }

                // generate where_clause
                if(this->is_valid && this->select_stat->whereClause) {
                    hsql::Expr* expr = this->select_stat->whereClause;
                    // std::cout << expr->type << std::endl;
                    // Operator 8, (only this type in test cases)
                    if(expr->type == hsql::kExprOperator) {
                        // solve the expr recursively
                        this->solve_expr(expr, this->select.where);
                    }
                }

                // print select statement
                if(this->is_valid) {
                    std::cout << this->select << std::endl;
                }
                break;

            // insert statement
            case hsql::kStmtInsert:
                this->sql_type = 2;
                // static_cast to hsql::InsertStatement*
                this->insert_stat = static_cast<hsql::InsertStatement*>(this->stat);

                // get rname
                if(this->insert_stat->tableName) {
                    this->insert.rname = lower_string(this->insert_stat->tableName);
                }

                // get values
                if(this->is_valid && this->insert_stat->values) {
                    int index = 0;
                    for (const hsql::Expr* expr : *this->insert_stat->values) {
                        // float type
                        if(expr->type == hsql::kExprLiteralFloat) {
                            this->insert.add_value(expr->fval);
                        }
                        // string type
                        else if(expr->type == hsql::kExprLiteralString) {
                            this->insert.add_value(expr->name);
                        }
                        // int type
                        else if(expr->type == hsql::kExprLiteralInt) {
                            this->insert.add_value(int(expr->ival));
                        }
                        // other type
                        else {
                            std::cout << "Now the system only supports data type of int, float and string." << std::endl;
                            this->is_valid = false;
                        }
                        index++;
                    }
                }

                // print insert statement
                if(this->is_valid) {
                    std::cout << this->insert << std::endl;
                }
                break;

            // delete statement
            case hsql::kStmtDelete:
                this->sql_type = 3;
                // static_cast to hsql::DeleteStatement*
                this->delete_stat = static_cast<hsql::DeleteStatement*>(this->stat);
                
                // get rname
                if(this->delete_stat->tableName) {
                    std::string rname = lower_string(this->delete_stat->tableName);
                    this->delete_s.rname = rname;
                }

                // get where clause
                if(this->is_valid && this->delete_stat->expr) {
                    hsql::Expr* expr = this->delete_stat->expr;
                    if(expr->type == hsql::kExprOperator) {
                        // solve the expr recursively
                        this->solve_expr(expr, this->delete_s.where);
                    }
                }

                // delete rname prefix
                for(auto& p : this->delete_s.where) {
                    p.left_name = p.left_name.substr(p.left_name.find_first_of(".")+1) ;
                }

                // print delete statement
                if(this->is_valid) {
                    std::cout << this->delete_s << std::endl;
                }
                break;
            case hsql::kStmtCreate:
                this->sql_type = 4;
                this->create_stat = static_cast<hsql::CreateStatement*>(this->stat);
                this->create_table.name = this->create_stat->tableName;
                for(int i = 0;i<this->create_stat->columns[0].size();i++){
                    ColumnDef tmpcol;
                    tmpcol.name = string(this->create_stat->columns[0][i]->name);
                    tmpcol.type = type2string(this->create_stat->columns[0][i]->type);
                    tmpcol.desc = "";
                    tmpcol.key  = false;
                    this->create_table.cols.emplace_back(tmpcol);
                }
                for(int i = 0;i<this->create_stat->tableConstraints[0].size();i++){
                    string tmp = "";
                    for(int j = 0; j<this->create_stat->tableConstraints[0][i]->columnNames[0].size();j++){
                        tmp += string(this->create_stat->tableConstraints[0][i]->columnNames[0][j]) + ",";
                    }
                    tmp.erase(tmp.size()-1);
                    key_name.emplace_back(tmp);
                }
                for(auto &key:key_name){
                    for(auto &col:this->create_table.cols){
                        if(col.name == key)col.key = true;
                    }
                }
                break;
            case hsql::kStmtImport:
                this->sql_type = 5;
                this->import_stat = static_cast<hsql::ImportStatement*>(this->stat);
                this->import_s.filepath = string(this->import_stat->filePath);
                this->import_s.tablename = string(this->import_stat->tableName);
                break;
                
            default:
                std::cout << "Now the system only supports insert, select and delete statements." << std::endl;
                break;
        }
    } 
    else if(this->query.find("FRAGMENTATION")){
        this->sql_type = 6;
        Fragment newfragment = GetFragmentFromCreateFragment();
        if(saveFragToEtcd(newfragment)){
            this->createfrag_name = newfragment.name;       
        }
    }
    else{
        std::cout << this->result.errorMsg() << std::endl;
    }
}

// solve the expr recursively
void SQLProcessor::solve_expr(hsql::Expr* expr, std::vector<Predicate>& where) {
    // AND 19
    if(expr->opType == hsql::kOpAnd && this->is_valid) {
        this->solve_expr(expr->expr, where);
        this->solve_expr(expr->expr2, where);
    }
    else {
        std::string left_name;
        std::string op_type;
        std::string right_name;
        // = 10
        // eg. user.id=10; user.name='jiang'; user.id=creator.user_id
        if(expr->opType == hsql::kOpEquals) {
            if(expr->expr->getName() && expr->expr2->getName()) {
                // eg. user.name='jiang'
                if(expr->expr->type == hsql::kExprColumnRef && expr->expr2->type == hsql::kExprLiteralString) {
                    op_type = "=";
                    left_name = this->get_aname_from_expr(expr->expr);
                    right_name = expr->expr2->getName();
                    right_name = "'"+right_name+"'";
                    if(left_name != "") {
                        where.push_back(Predicate(PredicateType::filter, left_name, op_type, right_name));
                    } else {
                        this->is_valid = false;
                    }
                }
                // eg. 'jiang'=user.name
                else if(expr->expr2->type == hsql::kExprColumnRef && expr->expr->type == hsql::kExprLiteralString) {
                    op_type = "=";
                    // aname = expr->expr2->getName();
                    left_name = this->get_aname_from_expr(expr->expr2);
                    right_name = expr->expr->getName();
                    right_name = "'"+right_name+"'";
                    if(left_name != "") {
                        where.push_back(Predicate(PredicateType::filter, left_name, op_type, right_name));
                    } else {
                        this->is_valid = false;
                    }
                }
                // eg. user.id=creator.user_id
                else if(expr->expr->type == hsql::kExprColumnRef && expr->expr2->type == hsql::kExprColumnRef) {
                    op_type = "=";
                    left_name = this->get_aname_from_expr(expr->expr);
                    right_name = this->get_aname_from_expr(expr->expr2);
                    if(left_name != "" && right_name != "") {
                        where.push_back(Predicate(PredicateType::join, left_name, op_type, right_name));
                    } else {
                        this->is_valid = false;
                    }
                }
            } else if(expr->expr->getName()) {
                op_type = "=";
                // aname = expr->expr->getName();
                left_name = this->get_aname_from_expr(expr->expr);
                right_name = double2str(expr->expr2->ival);
                if(left_name != "") {
                    where.push_back(Predicate(PredicateType::filter, left_name, op_type, right_name));
                } else {
                    this->is_valid = false;
                }
            } else {
                op_type = "=";
                // aname = expr->expr2->getName();
                left_name = this->get_aname_from_expr(expr->expr2);
                right_name = double2str(expr->expr->ival);
                if(left_name != "") {
                    where.push_back(Predicate(PredicateType::filter, left_name, op_type, right_name));
                } else {
                    this->is_valid = false;
                }
            }
        }
        // < 12
        else if(expr->opType == hsql::kOpLess) {
            op_type = "<";
            // aname = expr->expr->getName();
            left_name = this->get_aname_from_expr(expr->expr);
            right_name = double2str(expr->expr2->ival);
            if(left_name != "") {
                where.push_back(Predicate(PredicateType::filter, left_name, op_type, right_name));
            } else {
                this->is_valid = false;
            }
        }
        // <= 13
        else if(expr->opType == hsql::kOpLessEq) {
            op_type = "<=";
            // aname = expr->expr->getName();
            left_name = this->get_aname_from_expr(expr->expr);
            right_name = double2str(expr->expr2->ival);
            if(left_name != "") {
                where.push_back(Predicate(PredicateType::filter, left_name, op_type, right_name));
            } else {
                this->is_valid = false;
            }
        }
        // > 14
        else if(expr->opType == hsql::kOpGreater) {
            op_type = ">";
            // aname = expr->expr->getName();
            left_name = this->get_aname_from_expr(expr->expr);
            right_name = double2str(expr->expr2->ival);
            if(left_name != "") {
                where.push_back(Predicate(PredicateType::filter, left_name, op_type, right_name));
            } else {
                this->is_valid = false;
            }
        }
        // >= 15
        else if(expr->opType == hsql::kOpGreaterEq) {
            op_type = ">=";
            // aname = expr->expr->getName();
            left_name = this->get_aname_from_expr(expr->expr);
            right_name = double2str(expr->expr2->ival);
            if(left_name != "") {
                where.push_back(Predicate(PredicateType::filter, left_name, op_type, right_name));
            } else {
                this->is_valid = false;
            }
        }
    }
}
Fragment SQLProcessor::GetFragmentFromCreateFragment() {
    Fragment fragment;
    string fragment_line = GetBetween(this->query, "(", ")");
    vector<string> fragment_list = GetList(fragment_line,",",")");
    fragment.fragtype = fragment_list[0];
    //cout << "fragtype :"<<fragment.fragtype <<endl;
    fragment.name = GetBetween(this->query, "CREATE FRAGMENTATION", "(");
    //cout << "name " << fragment.name<< endl;
    for (int i = 1; i < fragment_list.size(); i++) {
        string fragement_tmp = fragment_list[i];
        vector<string> fragment_tmp_list = GetList(fragement_tmp, " ", ")");
        FragDef frags_tmp;
        for(int j = 1;j<fragment_tmp_list.size()-1;j++){
            frags_tmp.condition.emplace_back(fragment_tmp_list[j]);
        }
        //cout<<fragment_tmp_list[fragment_tmp_list.size()-1]<<endl;
        frags_tmp.id = stoi(fragment_tmp_list[0]);
        frags_tmp.site = stoi(fragment_tmp_list[fragment_tmp_list.size()-1]);
        frags_tmp.size = 0;
        for (int j = 0; j <  frags_tmp.condition.size(); j++) {
            if (frags_tmp.condition[j].find(">=") != -1) {
                frags_tmp.column.emplace_back(GetBefore(frags_tmp.condition[j],">="));
            }
            else if (frags_tmp.condition[j].find("<=") != -1) {
                frags_tmp.column.emplace_back(GetBefore(frags_tmp.condition[j],"<="));
            }
            else if (frags_tmp.condition[j].find("=") != -1) {
                frags_tmp.column.emplace_back(GetBefore(frags_tmp.condition[j],"="));
            }
            else if (frags_tmp.condition[j].find(">") != -1) {
                frags_tmp.column.emplace_back(GetBefore(frags_tmp.condition[j],">"));
            }
            else if (frags_tmp.condition[j].find("<") != -1) {
                frags_tmp.column.emplace_back(GetBefore(frags_tmp.condition[j],"<"));
            }
            else {
                frags_tmp.column.emplace_back(frags_tmp.condition[j]);
            }
        }
        fragment.fragcolumns.insert(frags_tmp.column.begin(),frags_tmp.column.end());
        fragment.frags.push_back(frags_tmp);
    }
    //cout << "fragment.frags" << endl;
    //Traversefrags(fragment.frags);
    fragment.fragnum = fragment.frags.size();
    //cout << "fragment.fragnum" << fragment.fragnum << endl;
    return fragment;
}
std::string SQLProcessor::get_aname_from_expr(hsql::Expr* expr) {
    std::string aname;
    std::string rname;
    // eg. select name,id from user;
    if(expr->type == hsql::kExprColumnRef) {
        if(expr->hasTable()) {
            // eg. select user.id from user;
            rname = lower_string(expr->table);
        } else {
            // eg. select id from user;
            if(this->sql_type == 1 && this->select_stat->fromTable->type == hsql::kTableName) {
                rname = lower_string(this->select_stat->fromTable->getName());
            } else if (this->sql_type == 3) {
                rname = lower_string(this->delete_s.rname);
            }
        }
        aname = lower_string(expr->getName());
    }
    return rname + std::string(".") + aname;
}
