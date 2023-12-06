#include "Server.cpp"


int main()
{
	setlocale(LC_ALL, "");

	vector<string> nameItems;
	vector<string> urlImageItems;
	vector<int> priceItems;
	vector<int> countOfVout;

	gPages gpage;
	gpage.getPages();

	nameItems = gpage.nameItems;
	urlImageItems = gpage.urlImageItems;
	priceItems = gpage.priceItems;
	countOfVout = gpage.countOfVout;

	Server server;

	server.nameItems = nameItems;
	server.urlImageItems = urlImageItems;
	server.priceItems = priceItems;
	server.countOfVout = countOfVout;

	server.countOfItems = gpage.countOfItems;

	server.startServer();

	system("pause>null");
	
}