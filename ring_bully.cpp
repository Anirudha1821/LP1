#include <bits/stdc++.h>
#include <unistd.h> // for usleep
using namespace std;

struct Node
{
    int id;
    bool alive;
};
void ring(vector<Node>& nodes)
{
    int n=nodes.size();

    int max_id_node = 0;
    for (int i = 0; i < n; i++)
    {
        if (nodes[i].id > nodes[max_id_node].id)
        {
            max_id_node = i;
        }
    }
    cout << "Node " << max_id_node + 1 << " is the current Coordinator" << endl;
    usleep(500000);
    cout << "Node " << max_id_node + 1 << " failed." << endl;
    usleep(500000);
    nodes[max_id_node].alive = false;
    
    int detector = max_id_node;
    while (detector == max_id_node)
    {
        detector = rand() % n;
    }
    cout << "\nNode " << detector + 1 << " detected failure."<< endl;
    usleep(500000);
    cout << "\nNode " << detector + 1 << "starts the election."<< endl;
    usleep(500000);

    int curr = (detector + 1) % n;//vimp to increase by 1
    vector<int> id_seq;
    id_seq.push_back(nodes[detector].id);
    int prev = detector;
    int count = 0;
    while (curr != detector || count == 0)
    {

        cout << "Message passed from node " << prev + 1 << " to " << curr + 1 << " : [ ";
        for (auto i : id_seq)
        {
            cout << i << " ";
        }
        cout << "]" << endl;
        usleep(500000);
        if (nodes[curr].alive == true)
        {
            id_seq.push_back(nodes[curr].id);
        }

        prev = curr;
        curr++;
        curr = curr % (nodes.size());
        count++;
    }
    cout << "Now, node " << detector + 1 << " who initiated the election process has recieved its message back containing all the ids of the alive nodes" << endl;
    usleep(500000);
    cout << "Node " << detector + 1 << " detects highest id number from list and elects it as the new coordinator." << endl;
    usleep(500000);
    int max_id = *max_element(id_seq.begin(), id_seq.end());
    int coord = 0;
    for (int i = 0; i < n; i++)
    {
        if (nodes[i].id == max_id)
        {
            coord = i;
            break;
        }
    }
    cout << "Node " << coord + 1 << " is the new coordinator" << endl;
    usleep(500000);
}
void bully(vector<Node> &nodes)
{
    int n = nodes.size();

    // Find the node with the maximum ID (initial coordinator)
    int max_id_node = 0;
    for (int i = 0; i < n; ++i)
    {
        if (nodes[i].id > nodes[max_id_node].id)
        {
            max_id_node = i;
        }
    }

    cout << "Node " << max_id_node + 1 << " is the initial Coordinator" << endl;

    usleep(500000); // 0.5 sec waiting to go to next line

    cout << "Node " << max_id_node + 1 << " failed." << endl;
    nodes[max_id_node].alive = false;

    // Randomly select a node to detect the failure
    int detector = max_id_node;
    while (detector == max_id_node)
    {
        detector = rand() % n;
    }

    cout << "\nNode " << detector + 1 << " detected failure.\n"
         << endl;
    usleep(500000);

    cout << "\nNode " << detector + 1 << " starts the election.\n"
         << endl;
    usleep(500000);

    int current = detector;
    int count = 1;

    while (count != 0)
    {

        // for loop for sending election message to all higher nodes
        for (int i = 0; i < n; ++i)
        {
            if (nodes[i].id > nodes[current].id && nodes[i].alive)
            {
                cout << "\nNode " << current + 1 << " sends election message to node " << i + 1 << endl;
                usleep(500000);
            }
        }

        int mini = INT_MAX;
        int mini_ind = 0;
        count = 0; // vimp

        for (int i = 0; i < n; ++i)
        {
            if (nodes[i].id > nodes[current].id && nodes[i].alive)
            {
                cout << "\nNode " << i + 1 << " sends OK message to node " << current + 1 << endl;
                usleep(500000);

                if (nodes[i].id < mini)
                {
                    mini = nodes[i].id;
                    mini_ind = i;
                }

                count++; // vimp here only
            }
        }

        if (count != 0)
        {
            current = mini_ind;
        }
    }

    cout << "Node " << current + 1 << " doesn't get any more OK responses. "
         << "It declares itself as the new coordinator." << endl;

    cout << "Node " << current + 1 << " is the new coordinator" << endl;
}

int main()
{
    srand(time(0)); // vvimp
    vector<Node> nodes = {{1, true}, {2, true}, {3, true}, {4, true}, {5, true}};
    bully(nodes);
    cout<<"---------------------------------------------------------------------"<<endl;
    cout<<"---------------------------------------------------------------------"<<endl;
    ring(nodes);
    return 0;
}
