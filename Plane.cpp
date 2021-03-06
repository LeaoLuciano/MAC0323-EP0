#include "Plane.h"

using namespace std;

Plane::Plane()
    : fuel(0),
      flyTime(0),
      timeWaiting(0),
      planeNumber(0),
      priority(0),
      company(""),
      destiny(""),
      flying(false),
      VIP(false) {}

// O parametro name deve ser do padrão AA00AAA onde A indica uma letra e 0
// indica um dígito
Plane::Plane(string name, int flyTime, bool VIP, bool flying, int fuel)
    : fuel(fuel), flyTime(flyTime), VIP(VIP), flying(flying), timeWaiting(0) {
	setName(name);
	updatePriority();
}

Plane::Plane(AirCompany airCompany, bool VIP, bool flying, int fuel)
    : VIP(VIP), fuel(fuel), flying(flying), timeWaiting(0) {
	string name = "";
	int destinationNum = rand() % airCompany.getNumDestinations();

	name += airCompany.getName();
	int x = rand() % 1000;
	if (x / 100 == 0) name += '0';
	if (x / 10 == 0) name += '0';
	name += to_string(x);
	name += airCompany.getDestination(destinationNum)->getName();
	setName(name);

	flyTime = airCompany.getDestination(destinationNum)->getDistance();
	updatePriority();
}

void Plane::setName(string name) {
	company = name.substr(0, 2);
	planeNumber = stoi(name.substr(2, 3));
	destiny = name.substr(5, 3);
}

string Plane::getName() {
	string name = "";
	name += company;
	if (planeNumber / 100 == 0) name += '0';
	if (planeNumber / 10 == 0) name += '0';
	name += to_string(planeNumber);
	name += destiny;
	return name;
}

void Plane::update() {
	timeWaiting++;

	if (flying) {
		if (fuel < avgTimeToLeaveQueue) VIP = true;
		fuel--;
	} else if (timeWaiting > 0.1 * flyTime)
		VIP = true;
	avgTimeToLeaveQueue--;
	updatePriority();
}

void Plane::updatePriority() { priority = (VIP) ? EMERGENCY : timeWaiting; }

bool Plane::isFlying() { return flying; }

int Plane::getFuel() { return fuel; }

int Plane::getTimeWaiting() { return timeWaiting; }

int Plane::getAvgTimeToLeaveQueue() { return avgTimeToLeaveQueue; }

void Plane::setAvgTimeToLeaveQueue(int t_time) {
	this->avgTimeToLeaveQueue = t_time;
}

bool Plane::isVIP() { return VIP; }

void Plane::setVIP() { this->VIP = true; }

bool Plane::operator<(Plane &p) {
	return (this->VIP ^ p.VIP) ? p.VIP : this->timeWaiting < p.timeWaiting;
}

bool Plane::operator<=(Plane &p) {
	return (this->VIP ^ p.VIP) ? p.VIP : this->timeWaiting <= p.timeWaiting;
}

bool Plane::operator>(Plane &p) {
	return (this->VIP ^ p.VIP) ? this->VIP : this->timeWaiting > p.timeWaiting;
}

bool Plane::operator>=(Plane &p) {
	return (this->VIP ^ p.VIP) ? this->VIP : this->timeWaiting >= p.timeWaiting;
}

// Overload no operador << para poder imprimir um avião usando cout
ostream &operator<<(ostream &os, Plane &p) {
	os << "Avião " << p.getName() << ":" << (p.isVIP() ? "\t VIP" : " ") << endl;
	os << "\tEsperando por " << p.timeWaiting << " unidades de tempo" << endl;
	os << "\tTempo estimado para sair da fila " << p.avgTimeToLeaveQueue
	   << " unidades de tempo" << endl;
	if (p.isFlying())
		os << "\tCombustível - " << p.fuel << endl;
	else
		os << "\tTempo de voo estimado - " << p.flyTime << endl;
	// os << "\tPrioridade - " << p.priority << endl;
	return os;
}

Plane *createRandomPlane() {
	// Companhia aérea aleatória
	AirCompany *comp = &existingCompanies[rand() % numExistingCompanies];
	bool VIP =
	    (rand() % 100 < EMERGENCY_RATE); // EMERGENCY_RATE % de chance de ser VIP
	bool flying = (rand() % 100 < 50);   // 50% de chance de estar voando
	int fuel = rand() % MAX_FUEL;        // Combustível aleatório menor
	                                     // que o máximo possível

	return new Plane(*comp, VIP, flying, fuel);
}

Plane *createUserPlane() {
	string dest, comp;
	int number, fuel, travelTime;
	bool VIP, flying;
	char c;
	string name;

	system("clear");
	cout << "Criando avião para adicionar ao aeroporto:\n";

	cout << "Digite o nome do avião (AA000): ";
	cin >> name;
	comp = name.substr(0, 2);
	number = stoi(name.substr(2, 3));
	cout << "Pouso ou decolagem (P/D)? ";
	cin >> c;
	flying = (c == 'p' || c == 'P');
	if (flying) {
		cout << "Quanto de combustível o avião tem no momento? ";
		cin >> fuel;
		dest = "GRU";
		travelTime = INT32_MAX;
	} else {
		cout << "Qual o tempo de vôo estimado? ";
		cin >> travelTime;
		cout << "Qual o destino (3 Letras): ";
		cin >> dest;
		fuel = INT32_MAX;
	}
	cout << "É uma emergência (S/N)? ";
	cin >> c;
	VIP = (c == 'S' || c == 's');

	name += dest;
	return new Plane(name, travelTime, VIP, flying, fuel);
}