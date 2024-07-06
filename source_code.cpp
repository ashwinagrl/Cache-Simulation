#include <bits/stdc++.h>
using namespace std;

struct cacheElements
{
    string tag;
    int valid;
    int dirty;
};
typedef struct cacheElements cacheElements;

string convertHexToBin(string hexdec)
{
    int i = 0;
    string ans = "";

    while (hexdec[i])
    {
        switch (hexdec[i])
        {
        case '0':
            ans = ans + "0000";
            break;
        case '1':
            ans = ans + "0001";
            break;
        case '2':
            ans = ans + "0010";
            break;
        case '3':
            ans = ans + "0011";
            break;
        case '4':
            ans = ans + "0100";
            break;
        case '5':
            ans = ans + "0101";
            break;
        case '6':
            ans = ans + "0110";
            break;
        case '7':
            ans = ans + "0111";
            break;
        case '8':
            ans = ans + "1000";
            break;
        case '9':
            ans = ans + "1001";
            break;
        case 'A':
        case 'a':
            ans = ans + "1010";
            break;
        case 'B':
        case 'b':
            ans = ans + "1011";
            break;
        case 'C':
        case 'c':
            ans = ans + "1100";
            break;
        case 'D':
        case 'd':
            ans = ans + "1101";
            break;
        case 'E':
        case 'e':
            ans = ans + "1110";
            break;
        case 'F':
        case 'f':
            ans = ans + "1111";
            break;
        case '.':
            ans = ans + ".";
            break;
        }
        i++;
    }
    int zeros_to_add = 32 - ans.length();
    for (int i = 0; i < zeros_to_add; i++)
    {
        ans = "0" + ans;
    }
    return ans;
}

string binToHex(string hex)
{
    int zeros_to_add; // Number of hex digits might not be disisible by 4 so we have to add 0s in the beginning
    if (hex.length() % 4 == 0)
    {
        zeros_to_add = 0;
    }
    else
    {
        zeros_to_add = 4 - (hex.length() % 4);
    }

    for (int i = 0; i < zeros_to_add; i++)
    {
        hex = "0" + hex;
    }
    string ans = "";
    int i = 0;
    while (i < hex.length())
    {
        string temp = hex.substr(i, 4);
        if (temp == "0000")
        {
            ans = ans + "0";
        }
        else if (temp == "0001")
        {
            ans = ans + "1";
        }
        else if (temp == "0010")
        {
            ans = ans + "2";
        }
        else if (temp == "0011")
        {
            ans = ans + "3";
        }
        else if (temp == "0100")
        {
            ans = ans + "4";
        }
        else if (temp == "0101")
        {
            ans = ans + "5";
        }
        else if (temp == "0110")
        {
            ans = ans + "6";
        }
        else if (temp == "0111")
        {
            ans = ans + "7";
        }
        else if (temp == "1000")
        {
            ans = ans + "8";
        }
        else if (temp == "1001")
        {
            ans = ans + "9";
        }
        else if (temp == "1010")
        {
            ans = ans + "A";
        }
        else if (temp == "1011")
        {
            ans = ans + "B";
        }
        else if (temp == "1100")
        {
            ans = ans + "C";
        }
        else if (temp == "1101")
        {
            ans = ans + "D";
        }
        else if (temp == "1110")
        {
            ans = ans + "E";
        }
        else if (temp == "1111")
        {
            ans = ans + "F";
        }
        i += 4;
    }
    return ans;
}

int binToDec(string bin)
{
    int ans = 0;
    int k = 1;
    for (int i = bin.size() - 1; i >= 0; i--)
    {
        ans = ans + k * ((int)bin[i] - (int)'0');
        k *= 2;
    }
    return ans;
}

void deal_FIFO(int number_of_sets, int tag_bits, int set_bits, int associativity, string replacement_policy, string writeback_policy, vector<pair<string, string>> input_vector)
{
     // Initializing the cache.
    vector<vector<cacheElements *>> FIFO_cache(number_of_sets, vector<cacheElements *>(associativity));
    for (int i = 0; i < number_of_sets; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            cacheElements *temp = new cacheElements;
            temp->valid = 0;
            FIFO_cache[i][j]= temp;
        }
    }
    vector<pair<int, int>> fifo_pointers(number_of_sets, make_pair(0, 0)); // vector to store pointers for fifo access.

    for (int i = 0; i < input_vector.size(); i++)
    {

        string mode = input_vector[i].first;
        string address = input_vector[i].second;

        string tag = address.substr(0, tag_bits);
        string set = address.substr(tag_bits, set_bits);
        int set_number;
        if (set_bits != 0)
        {
            set_number = binToDec(set);
        }
        else
        {
            set_number = 0;
        }
        bool hit = false;
        int hit_index;
        for (int i = 0; i < associativity; i++)
        {
            if (FIFO_cache[set_number][i]->valid == 1 && FIFO_cache[set_number][i]->tag == tag)
            {
                hit = true;
                hit_index = i;
                break;
            }
        }
        // cout<<"here"<<endl;
        if (hit)
        {
            cout << "Address: 0x" << binToHex(address) << ", Set: 0x" << binToHex(set) << ", Hit, Tag: 0x" << binToHex(tag) << endl;

            if (mode == "W" && writeback_policy == "WB")
            {
                FIFO_cache[set_number][hit_index]->dirty = 1;
            }
        }
        else
        {
            cout << "Address: 0x" << binToHex(address) << ", Set: 0x" << binToHex(set) << ", Miss, Tag: 0x" << binToHex(tag) << endl;

            if ((mode == "W" && writeback_policy == "WB") || (mode == "R"))
            {
                int index_of_empty_location = -1;

                bool is_line_full = true;
                for (int i = 0; i < associativity; i++)
                {
                    if (FIFO_cache[set_number][i]->valid == 0)
                    {
                        index_of_empty_location = i;
                        is_line_full = false;
                        break;
                    }
                }

                if (is_line_full)
                {
                    int index_to_insert = fifo_pointers[set_number].second;

                    FIFO_cache[set_number][index_to_insert]->tag = tag;
                    FIFO_cache[set_number][index_to_insert]->valid = 1;
                    FIFO_cache[set_number][index_to_insert]->dirty = 0;

                    fifo_pointers[set_number].second = (fifo_pointers[set_number].second + 1) % associativity; // Updating the front pointer

                    fifo_pointers[set_number].first = (fifo_pointers[set_number].first + 1) % associativity; // Updating the back pointer
                }
                else
                {
                    int index_to_insert = fifo_pointers[set_number].second;

                    FIFO_cache[set_number][index_to_insert]->tag = tag;
                    FIFO_cache[set_number][index_to_insert]->valid = 1;
                    FIFO_cache[set_number][index_to_insert]->dirty = 0;

                    fifo_pointers[set_number].second = (fifo_pointers[set_number].second + 1) % associativity; // Updating the front pointer
                }
            }
        }
    }
}
void deal_LRU(int number_of_sets, int tag_bits, int set_bits, int associativity, string replacement_policy, string writeback_policy, vector<pair<string, string>> input_vector)
{
    // Initializing the cache.
    vector<vector<cacheElements *>> LRU_cache(number_of_sets, vector<cacheElements *>(associativity));
    for (int i = 0; i < number_of_sets; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            cacheElements *temp = new cacheElements;
            temp->valid = 0;
            LRU_cache[i][j]= temp;
        }
    }

    vector<vector<int>> access_time(number_of_sets, vector<int>(associativity, 0)); // vector to store access time.

    for (int i = 0; i < input_vector.size(); i++)
    {
        string mode = input_vector[i].first;
        //cout << mode << endl;
        string address = input_vector[i].second;

        string tag = address.substr(0, tag_bits);
        string set = address.substr(tag_bits, set_bits);

        int set_number;
        if (set_bits != 0)
        {
            set_number = binToDec(set);
        }
        else
        {
            set_number = 0;
        }

        bool hit = false;
        int hit_index = -1;
        
        for (int i = 0; i < associativity; i++)
        {
            if ((LRU_cache[set_number][i]->valid == 1) && (LRU_cache[set_number][i]->tag == tag))
            {
                hit = true;
                hit_index = i;
                break;
            }
        }

        if (hit)
        {
            cout << "Address: 0x" << binToHex(address) << ", Set: 0x" << binToHex(set)<< ", Hit, Tag: 0x" << binToHex(tag) << endl;

            if (mode == "W" && writeback_policy == "WB")
            {
                LRU_cache[set_number][hit_index]->dirty = 1;
            }
            access_time[set_number][hit_index] = i;
        }
        else
        {
            // This is the case when there is a miss
            cout << "Address: 0x" << binToHex(address) << ", Set: 0x" << binToHex(set) << ", Miss, Tag: 0x" << binToHex(tag) << endl;

            if ((mode == "W" && writeback_policy == "WB") || (mode == "R"))
            {
                // Now we have to insert the element in the cache

                int index_of_empty_location = -1;

                bool is_line_full = true;
                for (int i = 0; i < associativity; i++)
                {
                    if (LRU_cache[set_number][i]->valid == 0)
                    {
                        index_of_empty_location = i;
                        is_line_full = false;
                        break;
                    }
                }

                if (is_line_full)
                {
                    int min_time = INT_MAX;
                    int min_index = -1;

                    for (int i = 0; i < associativity; i++)
                    {
                        if (access_time[set_number][i] < min_time)
                        {
                            min_time = access_time[set_number][i];
                            min_index = i;
                        }
                    }

                    LRU_cache[set_number][min_index]->tag = tag;
                    LRU_cache[set_number][min_index]->valid = 1;
                    LRU_cache[set_number][min_index]->dirty = 0;

                    access_time[set_number][min_index] = i; // Updating the access time.
                }
                else
                {
                    LRU_cache[set_number][index_of_empty_location]->tag = tag;
                    LRU_cache[set_number][index_of_empty_location]->valid = 1;
                    LRU_cache[set_number][index_of_empty_location]->dirty = 0;

                    access_time[set_number][index_of_empty_location] = i; // Updating the access time.
                }
            }
        }
    }
}

void deal_Random(int number_of_sets, int tag_bits, int set_bits, int associativity, string replacement_policy, string writeback_policy, vector<pair<string, string>> input_vector)
{
     // Initializing the cache.
    vector<vector<cacheElements *>> random_cache(number_of_sets, vector<cacheElements *>(associativity));
    for (int i = 0; i < number_of_sets; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            cacheElements *temp = new cacheElements;
            temp->valid = 0;
            random_cache[i][j]= temp;
        }
    }

    for (int i = 0; i < input_vector.size(); i++)
    {
        string mode = input_vector[i].first;
        string address = input_vector[i].second;

        string tag = address.substr(0, tag_bits);
        string set = address.substr(tag_bits, set_bits);
        int set_number;
        if (set_bits != 0)
        {
            set_number = binToDec(set);
        }
        else
        {
            set_number = 0;
        }

        bool hit = false;
        int hit_index;
        for (int i = 0; i < associativity; i++)
        {
            if (random_cache[set_number][i]->valid == 1 && random_cache[set_number][i]->tag == tag)
            {
                hit = true;
                hit_index = i;
                break;
            }
        }

        if (hit)
        {
            cout << "Address: 0x" << binToHex(address) << ", Set: 0x" << binToHex(set) << ", Hit, Tag: 0x" << binToHex(tag) << endl;
            if (mode == "W" && writeback_policy == "WB")
            {
                random_cache[set_number][hit_index]->dirty = 1;
            }
        }
        else
        {
            // This is the case when there is a miss
            cout << "Address: 0x" << binToHex(address) << ", Set: 0x" << binToHex(set) << ", Miss, Tag: 0x" << binToHex(tag) << endl;

            // Now we have to insert the element in the cache
            if ((mode == "W" && writeback_policy == "WB") || (mode == "R"))
            {
                bool is_line_full = true;
                int index_of_empty_location = -1;
                for (int i = 0; i < associativity; i++)
                {
                    if (random_cache[set_number][i]->valid == 0)
                    {
                        index_of_empty_location = i;
                        is_line_full = false;
                        break;
                    }
                }

                if (is_line_full)
                {
                    int random_index = rand() % associativity;
                    random_cache[set_number][random_index]->tag = tag; // The valid bit would have been already 1.
                    random_cache[set_number][random_index]->dirty = 0;
                }
                else
                {
                    random_cache[set_number][index_of_empty_location]->tag = tag;
                    random_cache[set_number][index_of_empty_location]->valid = 1;
                    random_cache[set_number][index_of_empty_location]->dirty = 0;
                }
            }
        }
    }
}
int main()
{
    int size_of_cache;
    int block_size;
    int associativity;
    string replacement_policy;
    string writeback_policy;

    fstream my_file;
    my_file.open("cache.config");

    if (my_file.is_open())
    {
        my_file >> size_of_cache >> block_size >> associativity >> replacement_policy >> writeback_policy;
        my_file.close(); // Close the file after reading
    }
    else
    {
        cout << "Error opening the file." << endl;
        return 0;
    }
    my_file.close();

    vector<pair<string, string>> input_vector; // Here the first element of the pair is the mode and the second element is the address

    my_file.open("cache.access");
    // Reading the file line by line;
    if (my_file.is_open())
    {
        string line;
        while (getline(my_file, line))
        {
            string mode = line.substr(0, 1);
            string address = convertHexToBin(line.substr(5));
            input_vector.push_back(make_pair(mode, address));
        }
        my_file.close(); // Close the file after reading
    }
    else
    {
        cout << "Error opening the file." << endl;
        return 0;
    }

    if (associativity == 0)
    { // Case of fully associative cache
        associativity = size_of_cache / block_size;
    }

    int number_of_sets = size_of_cache / (block_size * associativity);
    int set_bits = log2(number_of_sets);
    int offset_bits = log2(block_size);
    int tag_bits = 32 - set_bits - offset_bits; // It was clearly mentioned in the assignment pdf that we can assume 32 bit address always

    if (replacement_policy == "LRU")
    {
        deal_LRU(number_of_sets, tag_bits, set_bits, associativity, replacement_policy, writeback_policy, input_vector);
    }
    else if (replacement_policy == "FIFO")
    {
        deal_FIFO(number_of_sets, tag_bits, set_bits, associativity, replacement_policy, writeback_policy, input_vector);
    }
    else if (replacement_policy == "RANDOM")
    {
        srand(time(NULL));
        deal_Random(number_of_sets, tag_bits, set_bits, associativity, replacement_policy, writeback_policy, input_vector);
    }

    return 0;
}