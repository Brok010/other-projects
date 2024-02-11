#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variables
char date1[] = "2024/1/28 11:38:21";
char date2[] = "2024/2/11 11:38:21";
int step_min = 30; // Step in minutes

void generate_values(float *temperature, float *humidity) {
    static float prev_temperature = -1; // Previous temperature value
    static float prev_humidity = -1;    // Previous humidity value

    // Generate random initial values for temperature and humidity
    if (prev_temperature == -1 && prev_humidity == -1) {
        *temperature = (float)(rand() % 510) / 10.0; // Random value between 0 and 50 with one decimal place
        *humidity = (float)(rand() % 610 + 200) / 10.0; // Random value between 20 and 80 with one decimal place
    } else {
        // Generate values within 1 degree of the previous value for temperature
        *temperature = prev_temperature + (((float)(rand() % 2001) / 1000.0) - 1.0); // Random value between -1.0 and 1.0

        // Ensure temperature stays within 0 to 50 range
        if (*temperature < 0)
            *temperature = 0;
        else if (*temperature > 50)
            *temperature = 50;

        // Generate values within 1 unit of the previous value for humidity
        *humidity = prev_humidity + (((float)(rand() % 2001) / 1000.0) - 1.0); // Random value between -1.0 and 1.0

        // Ensure humidity stays within 20 to 80 range
        if (*humidity < 20)
            *humidity = 20;
        else if (*humidity > 80)
            *humidity = 80;
    }

    // Update previous values
    prev_temperature = *temperature;
    prev_humidity = *humidity;
}


void parse_date(const char *date_str, struct tm *tm_date) {
    sscanf(date_str, "%d/%d/%d %d:%d:%d",
           &tm_date->tm_year, &tm_date->tm_mon, &tm_date->tm_mday,
           &tm_date->tm_hour, &tm_date->tm_min, &tm_date->tm_sec);

    // Adjust for zero-based indexing of month and year counting from 1900
    tm_date->tm_mon -= 1;
    tm_date->tm_year -= 1900;
}

int main() {
    // Open file for writing
    FILE *fp;
    fp = fopen("data.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!");
        return 1;
    }

    // Seed random number generator
    srand(time(NULL));

    // Convert date strings to time structs
    struct tm tm_date1 = {0};
    struct tm tm_date2 = {0};

    // Parse date1 and date2 strings
    parse_date(date1, &tm_date1);
    parse_date(date2, &tm_date2);

    
    // Convert step to seconds
    int step_sec = step_min * 60;

    // Iterate through dates and write to file
    time_t current_time = mktime(&tm_date1);
    time_t end_time = mktime(&tm_date2);
    while (current_time <= end_time) {
        // Get current date and time
        struct tm *tm_current = localtime(&current_time);

        // Generate random temperature and humidity
        float temperature, humidity;
        generate_values(&temperature, &humidity);

        // Write to file
        fprintf(fp, "%04d/%02d/%02d %02d:%02d:%02d, %.2f, %.2f\n",
                tm_current->tm_year + 1900, tm_current->tm_mon + 1, tm_current->tm_mday,
                tm_current->tm_hour, tm_current->tm_min, tm_current->tm_sec,
                temperature, humidity);

        // Move to next time step
        current_time += step_sec;
    }

    // Close file
    fclose(fp);

    printf("Data written to data.csv successfully.\n");

    return 0;
}
