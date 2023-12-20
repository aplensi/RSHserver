#include "Parser.cpp"

class Server
{

	vector<SOCKET> Connections;
	gPages gItem;
	int counter = 0;

	public:
		vector<string> nameItems;
		vector<string> urlImageItems;
		vector<int> priceItems;
		vector<int> countOfVout;
		int countOfItems;

		void clientHandler(int index)
		{
			counter++;
			char rcv[256];
			memset(rcv, 0, 256);
			string srt,srtSize;
			int checker, hPt;
			while (true)
			{
				string Url = toLine(2);
				checker = recv(Connections[index], rcv, sizeof(rcv), 0);
				if (checker > 0)
				{
					cout << endl << rcv;
					if (rcv[0] == '0')
					{
						send(Connections[index], Url.c_str(), Url.size(), 0);
						memset(rcv, 0, 256);
					}
					if (rcv[0] == '1')
					{
						cout << " Names" << endl;
						srt = toLine(1);
						send(Connections[index], srt.c_str(), srt.size(), 0);
						memset(rcv, 0, 256);
					}
					if (rcv[0] == '2')
					{
						cout << " Urls" << endl;
						srt = toLine(2);
						send(Connections[index], srt.c_str(), srt.size(), 0);
						memset(rcv, 0, 256);
					}
					if (rcv[0] == '3')
					{
						cout << " Prices" << endl;
						srt = toLine(3);
						send(Connections[index], srt.c_str(), srt.size(), 0);
						memset(rcv, 0, 256);
					}
					if (rcv[0] == '5')
					{
						cout << " Votes" << endl;
						srt = toLine(5);
						send(Connections[index], srt.c_str(), srt.size(), 0);
						memset(rcv, 0, 256);
					}
					if (rcv[0] == 'j')
					{
						memset(rcv, 0, 256);
						srt = gItem.createPersonalCode();
						for (int i = 0; i < srt.size(); i++)
						{
							rcv[i] = srt[i];
						}
						send(Connections[index], rcv, sizeof(rcv), 0);
						cout << endl << rcv;
					}
					if (rcv[0] == 'k')
					{
						srt = string(rcv);
						string key = "";
						memset(rcv, 0, 256);
						srt.erase(0, 1);
						for (int i = 0; i < 80; i++)
						{
							key += srt[i];
						}
						srt.erase(0, 80);
						hPt = atoi(srt.c_str());
						cout << "\nKey = " << key << "\nNumber of element = " << hPt;
						srt = gItem.userVout(key, hPt, countOfVout, countOfItems);
						if (srt[0] == '+')
						{
							countOfVout[hPt]++;
						}
						else
						{
							countOfVout[hPt]--;
						}
						memset(rcv, 0, 256);
						rcv[0] = srt[0];
						send(Connections[index], rcv, sizeof(rcv), 0);
					}
					if (rcv[0] == 'c')
					{
						srt = string(rcv);
						string key = "", votes;
						memset(rcv, 0, 256);
						srt.erase(0, 1);
						votes = getVotes(srt);
						send(Connections[index], votes.c_str(), votes.size(), 0);
					}
					memset(rcv, 0, 256);
					cout << endl;
				}
				else
				{
					cout << "User disconected\n";
					break;
				}
			}
		}



		string toLine(int req)
		{
			string st = "";
			switch (req)
			{
			case 1:
				for (int i = 0; i < countOfItems; i++)
				{
					st += nameItems[i] + "|";
				}
				return st;
			case 2:
				for (int i = 0; i < countOfItems; i++)
				{
					st += urlImageItems[i] + "|";
				}
				return st;
			case 3:
				for (int i = 0; i < countOfItems; i++)
				{
					st += to_string(priceItems[i]) + "|";
				}
				return st;
			case 5:
				for (int i = 0; i < countOfItems; i++)
				{
					st += to_string(countOfVout[i]) + "|";
				}
				return st;
			default:
				break;
			}
		}
		
		string getVotes(string key)
		{
			string line;
			ifstream ifs;
			ifs.open("users/" + key + ".txt");
			if (!ifs.is_open())
			{
				ifs.close();
				ofstream ofs;
				ofs.open("users/" + key + ".txt");
				for (int i = 0; i << priceItems.size(); i++)
				{
					line += "-";
				}
				ofs << line;
				return line;
			}
			getline(ifs, line);
			ifs.close();
			return line;
		}

		void startServer()
		{
			WSADATA wsData;

			int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

			if (erStat != 0) {
				cout << "Error WinSock version initializaion #";
				cout << WSAGetLastError();
			}
			else
				cout << "\nWinSock initialization is OK" << endl;

			SOCKADDR_IN addr;
			int sizeofaddr = sizeof(addr);
			addr.sin_addr.s_addr = INADDR_ANY;
			addr.sin_port = htons(25565);
			addr.sin_family = AF_INET;

			SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
			bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
			listen(sListen, SOMAXCONN);

			char str[256];
			sprintf(str, "%d", countOfItems);

			SOCKET newConnection;
			do
			{
				newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

				if (newConnection == 0)
				{
					cout << "Error 2#" << endl;
				}
				else
				{
					char clientIp[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, (SOCKADDR*)&addr, clientIp, INET_ADDRSTRLEN);
					cout << "Client connected" << endl << "[IP: " << clientIp << "]" << endl;
					Connections.push_back(newConnection);
					send(Connections[counter], str, sizeof(str), 0);
					thread([&]() {
						int index = counter;
						cout << "\nThread started\nCounter: " << counter << "\n";
						clientHandler(counter);
						}).detach();
				}
			} while (true);
		}

};