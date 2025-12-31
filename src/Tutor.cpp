#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <cmath>

#include "Highway.h"

const int SECONDS_IN_HOURS = 3600;

//Queste var le metto globali come sono ora oppure le metto nel main e le passo per riferimento alle funzioni?????
double currentTime = 0;
std::unordered_map<std::string, std::vector<PassagePlateKey>> passagesPlateKey;  //POSSIBILE PROBLEMA DI MEMORIA??? TROPPI DATI 
std::unordered_map<int, std::vector<PassageIdKey>> passagesIdKey; 
const std::vector<HighwayNode> junctions;
const std::vector<HighwayNode> gates;

struct PassagePlateKey {
    int id;
    double time;
};

struct PassageIdKey {
    std::string plate;
    double time;
};

bool readFromFile(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        
        std::stringstream ss(line);
        std::string singleWord;
        std::vector<std::string> words;

        while (ss >> singleWord) {
            words.push_back(singleWord);
        }

        if (words.size() == 3) {
            try {
                int id = std::stoi(words[0]);
                std::string plate = words[1];
                double time = std::stod(words[3]);
                passagesPlateKey[plate].push_back({id, time});
                passagesIdKey[id].push_back({plate, time});
            } catch (...) {
                return false;
            }
        }
    }
}

std::string set_time(int addTime){

    if(addTime <= 0){
        throw std::runtime_error("Error! Invalid file format.");
    }

    double newTime = currentTime + addTime;
    std::string output = "\nInfrazioni commesse tra gli istanti "+currentTime+" e "+newTime+":";

    //Scorro la mappa con chiave plate, cosi da scorrere un veicolo per volta
    for (const auto& mapElement : passagesPlateKey) {

        //Scorro tutti i gate attraversati da una macchina precisa, analizzandone due consecutivi alla volta tramite gateIdx e gateIdx+1
        for(std::size_t gateIdx=0; gateIdx+1 < mapElement.second.size(); gateIdx++){

            double timeGate1 = mapElement.second[gateIdx].time;
            double timeGate2 = mapElement.second[gateIdx+1].time;
            int idGate1 = mapElement.second[gateIdx].id;
            int idGate2 = mapElement.second[gateIdx+1].id;

            //Controllo che tutti i due gate di riferimento siano dentro il range di tempo richiesto dal parametro addTime
            if(timeGate1 > currentTime && timeGate1 < newTime && timeGate2 > currentTime && timeGate2 > newTime){

                double distanceDifference = std::abs(gates[idGate1] - gates[idGate2]); 
                double timeDifference = std::abs(timeGate1 - timeGate2);
                double averageVelocity = distanceDifference / (timeDifference/SECONDS_IN_HOURS);

                if(averageVelocity > 130){
                    output += "\nInfrazione";
                    output += "\nTarga: "+mapElement.first+"\nTratta: varco "+idGate1+" - varco "+idGate2+"\nVelocità media: "+averageVelocity+"\nIstante di passaggio varco "+idGate1+": "+timeGate1+"\nIstante di passaggio varco "+idGate2+": "+timeGate2+"\n";
                }

            }
        }
    }

    if(output == "\nInfrazioni commesse tra gli istanti "+currentTime+" e "+newTime+":") output = "Nessuna infrazione.";

    currentTime = newTime;

    return output;
}

//Traduce il tempo ricevuto come parametro da stringa a intero
int decodeInput(const std::string& s){
    bool hasM = false;
    std::string numberPart;
    int result;

    //Controllo ultimo carattere e tolgo m
    if (s.back() == 'm') {
        hasM = true;
        numberPart = s.substr(0, s.size() - 1);
    } else {
        numberPart = s;
    }

    //La parte numerica deve esistere
    if (numberPart.empty()) throw std::runtime_error("Error! Invalid file format.");

    //Conversione
    try {
        result = std::stoi(numberPart);
    } catch (...) {
        throw std::runtime_error("Error! Invalid file format.");
    }
    

    //Se aveva m, moltiplico per 60 per convertire in secondi
    if (hasM) {
        result *= 60;
    }

    return result;
}


int main() {
    
    std::string input = "";
    std::string command = "";
    std::string parameter = "";

    //Lettura del file Highway.txt
    try {
        Highway highway("Data/Highway.txt");
        junctions = highway.getJunctions();
        gates = highway.getGates();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1; //Interrompe il main
    }

    //Lettura del file Passages.txt
    try {
        readFromFile("Data/Passages.txt");
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1; //Interrompe il main
    }

    std::cout << "------BENVENUTO IN TUTOR AUTOSTRADALE------";
    std::cout << "Inserisci un comando>>";

    while (std::cin >> input) {
        
        //Separo il comando da eventuali parametri
        size_t splitPoint = input.find(' ');
        if (splitPoint < input.size()) {
            command = input;
            parameter  = "";
        } else {
            command = input.substr(0, splitPoint);
            parameter  = input.substr(splitPoint + 1);
        }


        if (command == "set_time" && parameter != "") {
            try{
                std::cout << set_time(decodeInput(parameter));
            }
            catch(const std::runtime_error& e){
                std::cerr << e.what() << std::endl;
            }
            
            std::cout << "Inserisci un comando>>";
        } 
        else if (command == "stats" && parameter == "") {
            

            std::cout << "Inserisci un comando>>";
        } 
        else if (command == "reset" && parameter == "") {


            std::cout << "Inserisci un comando>>";
        }
        else if (command == "Q" || command == "q"){
            return 0;
        }
        else {
            std::cerr << "Errore! Commando invalido!";
            std::cout << "Inserisci un comando>>";
        }

    }
    return 0;

}


