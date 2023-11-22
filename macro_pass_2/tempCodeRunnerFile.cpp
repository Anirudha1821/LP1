    for(int i=1; i<mdt_macro_first_line.size(); i++){

        string curr = mdt_macro_first_line[i];

        if(kpdt.find(curr) != kpdt.end()){//finded
            parameters[i] = kpdt[curr];
        }else{
            parameters[i] = curr;//does not need actually
        }

    }