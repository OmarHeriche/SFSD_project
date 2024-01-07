#include <stdio.h>
#include <string.h>

void extractFieldValue(char *jsonString, char *fieldName, char *destination) {
    char *fieldStart = strstr(jsonString, fieldName);

    if (fieldStart != NULL) {
        fieldStart += strlen(fieldName) + 3; // Move to the beginning of the field value

        char *fieldEnd = strchr(fieldStart, '"'); // Find the end of the field value

        if (fieldEnd != NULL) {
            size_t fieldLength = fieldEnd - fieldStart;
            strncpy(destination, fieldStart, fieldLength);
            destination[fieldLength] = '\0'; // Null-terminate the field value
        }
    }
}

int main() {
    char jsonString[] = "{\"name\": \"omar\", \"surname\": \"heriche\", \"value\": \"03\"}";

    char nameValue[30]; // Adjust the size as needed

    extractFieldValue(jsonString, "\"name\"", nameValue);

    printf("Extracted name value: %s\n", nameValue);

    return 0;
}
