/**
 *
 * Authors: Carson Liebentritt and Ethan Clodfelter
 *
 * Date: 2024-11-14
 *
 * Function definitions for airport.h
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "airport.h"

void generateReports(Airport *airports, int n) {

  char* airportString;

  printf("Airports (original): \n");
  printf("==============================\n");
  printAirports(airports, n);

  printf("\nAirports By GPS ID: \n");
  printf("==============================\n");
  qsort(airports, n, sizeof(Airport), cmpByGPSId);
  printAirports(airports, n);

  printf("\nAirports By Type: \n");
  printf("==============================\n");
  qsort(airports, n, sizeof(Airport), cmpByType);
  printAirports(airports, n);

  printf("\nAirports By Name: \n");
  printf("==============================\n");
  qsort(airports, n, sizeof(Airport), cmpByName);
  printAirports(airports, n);

  printf("\nAirports By Name - Reversed: \n");
  printf("==============================\n");
  qsort(airports, n, sizeof(Airport), cmpByNameDesc);
  printAirports(airports, n);

  printf("\nAirports By Country/City: \n");
  printf("==============================\n");
  qsort(airports, n, sizeof(Airport), cmpByCountryCity);
  printAirports(airports, n);

  printf("\nAirports By Latitude: \n");
  printf("==============================\n");
  qsort(airports, n, sizeof(Airport), cmpByLatitude);
  printAirports(airports, n);

  printf("\nAirports By Longitude: \n");
  printf("==============================\n");
  qsort(airports, n, sizeof(Airport), cmpByLongitude);
  printAirports(airports, n);

  printf("\nAirports By Distance from Lincoln: \n");
  printf("==============================\n");
  qsort(airports, n, sizeof(Airport), cmpByLincolnDistance);
  printAirports(airports, n);

  printf("\nClosest Airport to Lincoln: \n");
  printf("==============================\n");
  airportString = airportToString(&airports[0]);
  if(airportString != NULL) {
    printf("%s\n", airportString);
  }
  else {
    printf("No airports found!\n");
  }

  printf("\nFurthest Airport from Lincoln: \n");
  printf("==============================\n");
  airportString = airportToString(&airports[n - 1]);
  if(airportString != NULL) {
    printf("%s\n", airportString);
  }
  else {
    printf("No airports found!\n");
  }

  printf("\nEast-West Geographic Center: \n");
  printf("==============================\n");
  qsort(airports, n, sizeof(Airport), cmpByLongitude);
  airportString = airportToString(&airports[n / 2]);
  if(airportString != NULL) {
    printf("%s\n", airportString);
  }
  else {
    printf("No airports found!\n");
  }

  printf("\nNew York, NY airport: \n");
  printf("==============================\n");
  int foundNewYork = 0;
  Airport* airportNewYork = filterByCity(airports, n, "New York", "US", &foundNewYork);
  if(airportNewYork == NULL) {
    printf("No New York airport found!\n");
  }
  else {
    printAirports(airportNewYork, foundNewYork);
    free(airportNewYork);
  }
  //if none found, print: "No New York airport found!\n"

  printf("\nLarge airport: \n");
  printf("==============================\n");
  int foundLarge = 0;
  Airport* airportLarge = filterByType(airports, n, "large_airport", &foundLarge);
  if(airportLarge == NULL) {
    printf("No large airport found!\n");
  }
  else {
    printAirports(airportLarge, foundLarge);
    free(airportLarge);
  }

  return;
}

Airport* createAirport(const char* gpsId,
                       const char* type,
                       const char* name,
                       double latitude,
                       double longitude,
                       int elevationFeet,
                       const char* city,
                       const char* countryAbbrv) {

    if(gpsId == NULL){
      fprintf(stderr, "ERROR: invalid input (gpsId)\n");
      return NULL;
    }
    if(type == NULL){
      fprintf(stderr, "ERROR: invalid input (type)\n");
      return NULL;
    }
    if(name == NULL){
      fprintf(stderr, "ERROR: invalid input (name)\n");
      return NULL;
    }
    if(city == NULL){
      fprintf(stderr, "ERROR: invalid input (city)\n");
      return NULL;
    }
    if(countryAbbrv == NULL){
      fprintf(stderr, "ERROR: invalid input (countryAbbrv)\n");
      return NULL;
    }

    if(latitude < -90 || latitude > 90) {
      printf("ERROR: latitude out of range [-90, 90]\n");
      return NULL;
    }
    if(longitude < -180 || longitude > 180) {
      printf("ERROR: longitude out of range [-180, 180]\n");
      return NULL;
    }

    Airport *airport = (Airport *) malloc(sizeof(Airport));

    initAirport(airport, gpsId, type, name, latitude, longitude, elevationFeet, city, countryAbbrv);

    return airport;
}

void initAirport(Airport* airport,
                 const char* gpsId,
                 const char* type,
                 const char* name,
                 double latitude,
                 double longitude,
                 int elevationFeet,
                 const char* city,
                 const char* countryAbbrv) {

    if(airport == NULL){
      fprintf(stderr, "ERROR: invalid input (airport)\n");
      return;
    }
    if(gpsId == NULL){
      fprintf(stderr, "ERROR: invalid input (gpsId)\n");
      return;
    }
    if(type == NULL){
      fprintf(stderr, "ERROR: invalid input (type)\n");
      return;
    }
    if(name == NULL){
      fprintf(stderr, "ERROR: invalid input (name)\n");
      return;
    }
    if(city == NULL){
      fprintf(stderr, "ERROR: invalid input (city)\n");
      return;
    }
    if(countryAbbrv == NULL){
      fprintf(stderr, "ERROR: invalid input (countryAbbrv)\n");
      return;
    }

    if(latitude < -90 || latitude > 90) {
    printf("ERROR: latitude out of range [-90, 90]\n");
    return;
    }

    if(longitude < -180 || longitude > 180) {
      printf("ERROR: longitude out of range [-180, 180]\n");
      return;
    }

    airport->gpsId = strdup(gpsId);

    airport->type = strdup(type);

    airport->name = strdup(name);

    airport->city = strdup(city);

    airport->countryAbbrv = strdup(countryAbbrv);

    airport->latitude = latitude;

    airport->longitude = longitude;

    airport->elevationFeet = elevationFeet;

    return;
}

char* airportToString(const Airport* a) {
  char temp[1000];
  //this formatting is required but the code may need to be adapted
  // to your definiion of your Airport structure.
  sprintf(temp, "%-8s %-15s %-20s %.2f %.2f %d %-10s %-2s", a->gpsId, a->type, a->name,
          a->latitude, a->longitude, a->elevationFeet, a->city,
          a->countryAbbrv);
  char* result = (char*)malloc(sizeof(char) * (strlen(temp) + 1));
  strcpy(result, temp);
  return result;
}

void printAirports(Airport *airports, int n) {

  for(int i=0; i<n; i++) {
    char *s = airportToString(&airports[i]);
    printf("%s\n", s);
    free(s);
  }

  return;
}

double getAirDistance(const Airport* origin, const Airport* destination) {
  if (origin == NULL || destination == NULL || origin->latitude < -90 || origin->latitude > 90 || destination->latitude < -90 || destination->latitude > 90 ||
  origin->longitude < -180 || origin->longitude > 180 || destination->longitude < -180 || destination->longitude > 180) {
    printf("given invalid input\n");
    return -1;
  }
  double latA = origin->latitude * (M_PI / 180.0);
  double latB = destination->latitude * (M_PI / 180.0);
  double delta = (destination->longitude - origin->longitude) * (M_PI / 180.0);
  int R = 6371;
  double d = acos(sin(latA) * sin(latB) + cos(latA) * cos(latB) * cos(delta)) * R;
  return d;
}

double getEstimatedTravelTime(const Airport* stops,
                              int size,
                              double aveKmsPerHour,
                              double aveLayoverTimeHrs) {
  double airDistance = 0.0;
  double flightTime = 0.0;
  for(int i = 0; i < size - 2; i++) {
    airDistance = getAirDistance(&stops[i], &stops[i + 1]);
    flightTime += airDistance / aveKmsPerHour;
    if (airDistance < 0) {
            return -1;
    }
    if(i < size - 3) {
      flightTime += aveLayoverTimeHrs;
    }
    }

  return flightTime;
}

int cmpByGPSId(const void* a, const void* b) {
  const Airport *a1 = (const Airport *)a;
  const Airport *a2 = (const Airport *)b;
  int result = strcmp(a1->gpsId, a2->gpsId);
  return result;
}

int cmpByType(const void* a, const void* b) {
  const Airport *a1 = (const Airport *)a;
  const Airport *a2 = (const Airport *)b;
  int result = strcmp(a1->type, a2->type);
  return result;
}

int cmpByName(const void* a, const void* b) {
  const Airport *a1 = (const Airport *)a;
  const Airport *a2 = (const Airport *)b;
  int result = strcmp(a1->name, a2->name);
  return result;
}

int cmpByNameDesc(const void* a, const void* b) {
  const Airport *a1 = (const Airport *)a;
  const Airport *a2 = (const Airport *)b;
  int result = strcmp(a2->name, a1->name);
  return result;
}

int cmpByCountryCity(const void* a, const void* b) {
  const Airport *a1 = (const Airport *)a;
  const Airport *a2 = (const Airport *)b;
  int result = strcmp(a1->countryAbbrv, a2->countryAbbrv);
  if (result == 0) {
    result = strcmp(a1->city, a2->city);
  }
  return result;
}

int cmpByLatitude(const void* a, const void* b) {
  Airport *a1 = (Airport *)a;
  Airport *a2 = (Airport *)b;
  if (a1->latitude < a2->latitude) {
    return 1;
  } else if (a1->latitude > a2->latitude) {
    return -1;
  } else {
    return 0;
  }
}

int cmpByLongitude(const void* a, const void* b) {
  Airport *a1 = (Airport *)a;
  Airport *a2 = (Airport *)b;
  if (a1->longitude > a2->longitude) {
    return 1;
  } else if (a1->longitude < a2->longitude) {
    return -1;
  } else {
    return 0;
  }
}

int cmpByLincolnDistance(const void* a, const void* b) {
  Airport *a1 = (Airport *)a;
  Airport *a2 = (Airport *)b;
  Airport lincoln = {"0R2", "", "", 40.846176, -96.75471, 1, "", ""};

  double distanceA1 = getAirDistance(&lincoln, a1);
  double distanceA2 = getAirDistance(&lincoln, a2);

  if (distanceA1 > distanceA2) {
    return 1;
  } else if (distanceA1 < distanceA2) {
    return -1;
  } else {
    return 0;
  }
}

Airport* filterByCity(Airport* airports, int n, char* city, char *countryAbbrv, int* output_size) {
  int foundCity = 0;
  for(int i = 0; i < n; i++) {
    if(strcmp(airports[i].city, city) == 0 && strcmp(airports[i].countryAbbrv, countryAbbrv) == 0) {
      foundCity++;
    }
  }

  if(foundCity == 0) {
    return NULL;
  }

  int j = 0;
  Airport *result = (Airport *) malloc(sizeof(Airport) * foundCity);
  for(int i = 0; i < n; i++) {
    if(strcmp(airports[i].city, city) == 0 && strcmp(airports[i].countryAbbrv, countryAbbrv) == 0) {
      result[j] = airports[i];
      j++;
    }
  }

  *output_size = foundCity;
  return result;
}

Airport* filterByType(Airport* airports, int n, char* type, int* output_size) {
  int foundType = 0;
  for(int i = 0; i < n; i++) {
    if(strcmp(airports[i].type, type) == 0) {
      foundType++;
    }
  }

  if(foundType == 0) {
    return NULL;
  }

  int j = 0;
  Airport *result = (Airport *) malloc(sizeof(Airport) * foundType);
  for(int i = 0; i < n; i++) {
    if(strcmp(airports[i].type, type) == 0) {
      result[j] = airports[i];
      j++;
    }
  }

  *output_size = foundType;
  return result;
}

