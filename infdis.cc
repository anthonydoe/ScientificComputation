#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::setbase;
using std::setfill;
using std::vector;
using std::setw;
using std::left;
using std::shared_ptr;
using std::make_shared;
using namespace std;


class Population {
private:
  int pop;
  vector<int> model_population;
  shared_ptr<Population> result{nullptr};
public:
  /*Population( int npopulation ) {
   pop = npopulation;
   };*/
  Population ( int npopulation )
  : pop(npopulation),
    model_population(vector<int>(pop))
  {};

  void create_pop(float vacc_probability) {
    //this method sets people as not infected (0) or infected (5), 5 days is the infected time period
    bool disease;
    bool vaccination;
    int check = 0; 

    for (int i=0; i<model_population.size(); i++) {
      disease = random_infection();
      vaccination = random_vaccination(vacc_probability);

      if (disease==1 && check==0) {
	model_population.at(i) = 5;
	check = 1; // can take out the check to infect multiple people in the initial population
      } else {
	model_population.at(i) = 0;
      }
      if (vaccination==1)
	model_population.at(i) = -2;
    }
  };

  bool random_infection() {
    //returns whether a person is infected (true) or not (false)
    //used in the update method -- for the update() method


    float bad_luck = (float) rand()/(float)RAND_MAX;
    if (bad_luck>.95) {
      return true;
    } else {
    return false;
    }
  };

  bool random_vaccination(float vacc_probability) {
    //returns whether or not a person is vaccinated (true) or not (false)
    //vaccination probability is inputted by the user
    //used in creating the population 

    float good_luck = (float) rand()/(float)RAND_MAX;
    if (good_luck>(1-vacc_probability)) {
      return true;
    } else {
      return false;
    }
  };

  bool random_infection_contagion(float inf_probability) {
    //returns whether a person is infected (true) or not (false).
    //the transmission probability inputted by the user
    //used in the update_contagion method -- for the "contagion" portion of the project

    float bad_luck = (float) rand()/(float)RAND_MAX;
    if (bad_luck>(1-inf_probability)) {
      return true;
    } else {
      return false;
    }
  };

  int random_person() {
    //returns a random value between 0 and the maximum population
    //the value return is the representation of a random person in the population

    int person = rand() % (pop) + 0;
    return person;
  }

  int count_infected() { 
    //this method goes through the population and counts the values (people) who
    //are marked greater than 0

    int number_of_infected = 0;
    for (int i=0;i<model_population.size();i++) {
      if (model_population.at(i)>0) {
	number_of_infected = number_of_infected + 1;
      }
    }
    return number_of_infected;
  };

  int count_healthy_susceptible() {
    //counts the people in the population who have not caught the disease but are still vulnerable
    //these people are represented by the value 0

    int number_of_healthy_susceptible = 0;
    for (int i=0; i<model_population.size();i++) {
      if (model_population.at(i)==0) {
	number_of_healthy_susceptible = number_of_healthy_susceptible + 1;
      }
    }
    return number_of_healthy_susceptible;
  };

  int count_recovered() {
    //counts the people in the population who have caught the disease but are now recovered and healthy
    //these people are represented by the value -1

    int number_of_recovered = 0;
    for (int i=0; i<model_population.size();i++) {
      if (model_population.at(i)==-1) {
	number_of_recovered = number_of_recovered + 1;
      }
    }
    return number_of_recovered;
  };

  int count_vaccinated() {
    //counts the people in the population who are vaccinated -- they cannot catch the disease
    //these people are represented by the value -2

    int number_of_vaccinated = 0;
    for (int i=0; i<model_population.size(); i++) {
      if (model_population.at(i) == -2) {
	number_of_vaccinated = number_of_vaccinated + 1;
      }
    }
    return number_of_vaccinated;
  }; 

  void update_spreading(float probability, int ppl_contact) {

    //this method is for the "spreading" portion of the project
    //it goes through the population and spread the disease to random people in the population
    //the random person is selected using the random_person() method
    //only people who have the disease can pass it
    //the disease can be passed to anyone who is has not recovered or vaccinated

    int iter = 0;
    for ( ; ;iter++) {
      int count = 0; //this variable counts the amount of sick people per day
      int last_day = 0;
      for (int i=0; i<model_population.size(); i++) {

	if (model_population.at(i)<0) //the case in which someone is recovered from the disease or vaccinated
	  continue;

	if (model_population.at(i)>1) {  //the case in which a person is sick -- they are able to transmit the disease
	  count++;
	  for (int contact=0; contact<ppl_contact; contact++) {
	    //each loop represents contact with another person
	    //this happens each day for a person who is sick

	    int person;  //this value represents a random person in the population
	    person = random_person();
	    bool contact_result;  //this variable represents the result of contact. whether or not the disease is transmitted
	    contact_result = random_infection_contagion(probability);
   
	    if (model_population.at(person)==0 && contact_result==1) {  //if the person is susceptible and the disease is transmitted, enter the loop
	      model_population.at(person) = 5;
	      count++;
	      continue; 
	    } else {
	      continue;
	    }

	  }
	  model_population.at(i) = model_population.at(i) - 1; //reduce the days a person is sick by 1 -- counting down
	  continue;
	}

	if (model_population.at(i)==0)  //the case in which someone is susceptible
	  continue;

	if (model_population.at(i)==1) {
	  count++;
	  last_day++;  //counting the amount of people who this is there last day sick
	  for (int contact=0; contact<ppl_contact; contact++) {  
            //this for loop brings the sick person (on their last day of sick) into contact with random people
	    int person;
	    person = random_person();
	    bool contact_result;
	    contact_result = random_infection_contagion(probability);
	    if (model_population.at(person)==0 && contact_result==1) {
	      model_population.at(person) = 5;
	      continue;
	    } else {
	      continue;
	    }
	  }
	  model_population.at(i) = -1;
	  continue;
	}
      }
      int number_of_people_infected;
      number_of_people_infected = count_infected();
      cout << "Day " << iter+1 << ", the number of people now infected is: " << number_of_people_infected+last_day << endl;
      if (count==0)
	break;
    }

  };
	 


  void print() {
    //this method is for people who want to view a physical representation of the population
    //prints the population in a block that is 60 values wide

    int count = 0;
    for (int i=0; i<model_population.size(); i++) {
      count++;
      if (model_population.at(i)==0) {
	cout << setw(2) << "?" << " ";
      } else if (model_population.at(i)>0) {
	cout << setw(2) << "+" << " ";
      } else {
	if (model_population.at(i)==-2) {
	  cout << setw(2) << "v" << " ";
	} else {
	  cout << setw(2) << "-" << " ";
	}
      }
      //cout << setw(2) << model_population.at(i) << " ";
      if (count==30) {
	cout << endl;
	count = 0;
      }
    }
    cout << endl;
  };

  vector<int> return_model_population() {
    return model_population;
  };

};




int main() {


  cout << "How large do you want the population" << endl;
  int user_input;
  cin >> user_input;

  cout << "What do you want as the transmission probability? (Enter a number between 0 and 1)" << endl;
  float inf_probability;
  cin >> inf_probability;

  cout << "What do you want as the vaccination probability? (Enter a number between 0 and 1)" << endl;
  float vacc_probability;
  cin >> vacc_probability;

  cout << "How many people will each person in the simulation come into contact with per day?" << endl;
  int people_contact;
  cin >> people_contact;

  cout << "*********" << endl;
  
  Population population2(user_input);
  population2.create_pop(vacc_probability);
  cout << "Now updating the SPREADING portion of the project" << endl;
  population2.update_spreading(inf_probability, people_contact); 
  cout << "Susceptible but healthy people in the population: " << population2.count_healthy_susceptible() << endl;
  cout << "Vaccinated people in the population: " << population2.count_vaccinated() << endl;
  cout << "People who became sick and recovered in the population: " << population2.count_recovered() << endl;
  //population2.print();  
  //cout << "Final population" << endl;
  //population.print();

  cout << "Simulation over." << endl;


  return 0;
}
