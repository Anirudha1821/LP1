
#include <bits/stdc++.h>
using namespace std;

class Job
{
private:
    int id;
    string name;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;

public:
    Job()
    {
        id = 0;
        name = "";
        at = 0;
        bt = 0;
        ct = 0;
        tat = 0;
        wt = 0;
    }

    Job(int id, string name, int at, int bt)
    {
        this->id = id;
        this->name = name;
        this->at = at;
        this->bt = bt;
        ct = 0;
        tat = 0;
        wt = 0;
    }

    void operator=(const Job &j)
    {
        this->id = j.id;
        this->name = j.name;
        this->at = j.at;
        this->bt = j.bt;
        ct = 0;
        tat = 0;
        wt = 0;
    }

    friend class Schedular;
};

class Schedular
{
private:
    int n;
    Job *processes;//processes is a pointer to an array of objects of the Job
    vector<Job> readyQueue;

public:
    Schedular()
    {
        n = 0;
        processes = NULL;
    }
    Schedular(int n)
    {
        this->n = n;
        processes = new Job[n];
    }

    void input()
    {
        int a, b;
        for (int i = 0; i < n; i++)
        {
            cout << "Enter arrival time and burst time of process P" << (i + 1) << " : " << endl;
            cin >> a >> b;
            Job j(i + 1, "P" + to_string(i + 1), a, b);
            processes[i] = j;
        }
    }

    void display()
    {
        cout << "ID\tAT\tBT\tCT\tTAT\tWT" << endl;
        for (int i = 0; i < n; i++)
        {
            cout << processes[i].id << "\t" << processes[i].at << "\t" << processes[i].bt << "\t" << processes[i].ct << "\t" << processes[i].tat << "\t" << processes[i].wt << endl;
        }

        float sumTAT = 0;
        float sumWT = 0;
        for (int i = 0; i < n; i++)
        {
            sumTAT += processes[i].tat;
            sumWT += processes[i].wt;
        }

        float avgTAT = sumTAT / n;
        float avgWT = sumWT / n;
        cout << "Average turn around time : " << avgTAT << endl;
        cout << "Average waiting time : " << avgWT << endl;
    }

    void displayGantt(vector<Job> readyQueue, int currentTime)
    {
        cout << "Ready queue at current time " << currentTime << " sec : " << endl;
        for (int i = 0; i < readyQueue.size(); i++)
        {
            cout << readyQueue[i].name << "(" << readyQueue[i].bt << " sec)"
                 << " ";
        }
        cout << endl;
    }

    void fcfs()
    {
        int totalTime = 0;
        int currentTime = 0;
        int processCompleted = 0;

        sort(processes, processes + n, [](const Job &j1, const Job &j2)
             { return j1.at < j2.at; });

        for (int i = 0; i < n; i++)
        {
            totalTime += processes[i].bt;
        }

        while (currentTime <= totalTime)
        {
            if (processCompleted < n && processes[processCompleted].at <= currentTime)
            {
                readyQueue.push_back(processes[processCompleted]);
                processCompleted++;
            }

            this->displayGantt(readyQueue, currentTime);

            if(readyQueue.size() > 0)
            {
                readyQueue[0].bt--;
                if(readyQueue[0].bt == 0)
                {
                    for(int i = 0; i < n; i++)
                    {
                        if(readyQueue[0].id == processes[i].id)
                        {
                            processes[i].ct = currentTime + 1;
                            processes[i].tat = processes[i].ct - processes[i].at;
                            processes[i].wt = processes[i].tat - processes[i].bt;
                        }
                    }
                    readyQueue.erase(readyQueue.begin());
                }
            }
            currentTime++;
        }

        this->display();
    }

    void sjf()
    {
        int totalTime = 0;
        int currentTime = 0;
        int processCompleted = 0;

        sort(processes, processes + n, [](const Job &j1, const Job &j2)
             { return j1.at < j2.at; });

        for (int i = 0; i < n; i++)
        {
            totalTime += processes[i].bt;
        }

        while (currentTime <= totalTime)
        {
            if (processes[processCompleted].at <= currentTime && processCompleted < n)
            {
                readyQueue.push_back(processes[processCompleted]);
                processCompleted++;
            }

            sort(readyQueue.begin(), readyQueue.end(), [](const Job &j1, const Job &j2)
                 { return j1.bt < j2.bt; });

            this->displayGantt(readyQueue, currentTime);

            if (readyQueue.size() > 0)
            {
                readyQueue[0].bt--;
                if (readyQueue[0].bt == 0)
                {
                    for (int i = 0; i < n; i++)
                    {
                        if (readyQueue[0].id == processes[i].id)
                        {
                            processes[i].ct = currentTime + 1;
                            processes[i].tat = processes[i].ct - processes[i].at;
                            processes[i].wt = processes[i].tat - processes[i].bt;
                        }
                    }
                    readyQueue.erase(readyQueue.begin());
                }
            }
            currentTime++;
        }

        this->display();
    }

    void priority()
    {
        int totalTime = 0;
        int currentTime = 0;
        int processCompleted = 0;
        int priority[n];

        cout << "Enter priorities : " << endl;
        for (int i = 0; i < n; i++)
        {
            cin >> priority[i];
        }

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (priority[j] > priority[j + 1])
                {
                    swap(priority[j], priority[j + 1]);
                    swap(processes[j], processes[j + 1]);
                }
            }
        }

        for (int i = 0; i < n; i++)
        {
            totalTime += processes[i].bt;
        }

        while(currentTime <= totalTime)
        {
            if (processCompleted < n)
            {
                readyQueue.push_back(processes[processCompleted]);
                processCompleted++;
            }

            this->displayGantt(readyQueue, currentTime);

            if (readyQueue.size() > 0)
            {
                readyQueue[0].bt--;
                if (readyQueue[0].bt == 0)
                {
                    for (int i = 0; i < n; i++)
                    {
                        if (readyQueue[0].id == processes[i].id)
                        {
                            processes[i].ct = currentTime + 1;
                            processes[i].tat = (processes[i].ct - processes[i].at) > 0 ? processes[i].ct - processes[i].at : 0;
                            processes[i].wt = (processes[i].tat - processes[i].bt) > 0 ? processes[i].tat - processes[i].bt : 0;
                        }
                    }
                    readyQueue.erase(readyQueue.begin());
                }
            }
            currentTime++;
        }

        this->display();
    }

    void rr()
    {
        int totalTime = 0;
        int currentTime = 0;
        int processCompleted = 0;
        int addedBurst = 0;
        int interval;
        Job addedJob;
        addedJob.bt = 0;

        cout << "Enter time interval : ";
        cin >> interval;

        for (int i = 0; i < n; i++)
        {
            totalTime += processes[i].bt;
        }

        sort(processes, processes + n, [](const Job &j1, const Job &j2)
             { return j1.at < j2.at; });

        while (currentTime < totalTime)
        {
            while (processCompleted < n && processes[processCompleted].at <= currentTime)
            {
                readyQueue.push_back(processes[processCompleted]);
                processCompleted++;
            }

            if (addedJob.bt > 0)
            {
                readyQueue.push_back(addedJob);
            }

            if (readyQueue.size() > 0)
            {
                addedJob = readyQueue[0];
                readyQueue.erase(readyQueue.begin());
                if (addedJob.bt > interval)
                {
                    addedJob.bt -= interval;
                    addedBurst = interval;
                }
                else
                {
                    addedBurst = addedJob.bt;
                    addedJob.bt = 0;
                }

                if (addedJob.bt == 0)
                {
                    for (int i = 0; i < n; i++)
                    {
                        if (addedJob.id == processes[i].id)
                        {
                            processes[i].ct = currentTime + addedBurst;
                            processes[i].tat = processes[i].ct - processes[i].at;
                            processes[i].wt = processes[i].tat - processes[i].bt;
                        }
                    }
                }
            }
            currentTime += addedBurst;
        }
        this->display();
    }
};

int main()
{
    int n;
    int op;
    cout << "Enter number of processes : ";
    cin >> n;
    Schedular s(n);
    s.input();
    while (true)
    {
        cout << "MENU" << endl;
        cout << "1.First Come First Serve" << endl;
        cout << "2.Shortest Job First" << endl;
        cout << "3.Priority" << endl;
        cout << "4.Round Robin" << endl;
        cout << "5.Exit" << endl;
        cout << "Enter your choice : ";
        cin >> op;

        if (op == 1)
        {
            s.fcfs();
        }
        else if (op == 2)
        {
            s.sjf();
        }
        else if (op == 3)
        {
            s.priority();
        }
        else if (op == 4)
        {
            s.rr();
        }
        else
        {
            exit(0);
        }
    }
    return 0;
}