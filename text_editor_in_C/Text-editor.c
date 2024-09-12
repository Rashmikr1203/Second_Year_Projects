#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GAP_SIZE 1000

typedef struct 
{
	char* buffer;
	int gap_start;
	int gap_end;
	int size;
} GapBuffer;

void initGapBuffer(GapBuffer* gapBuffer, int initialSize) 
{
	gapBuffer->buffer = (char*)malloc(initialSize);
	gapBuffer->gap_start = 0;
	gapBuffer->gap_end = initialSize - 1;
	gapBuffer->size = initialSize;
}

void resizeGapBuffer(GapBuffer* gapBuffer) 
{
    int newSize = gapBuffer->size * 2;
    gapBuffer->buffer = (char*)realloc(gapBuffer->buffer, newSize);
    gapBuffer->gap_end = newSize - 1;
    memmove(gapBuffer->buffer + gapBuffer->gap_end - gapBuffer->size + 1,
    gapBuffer->buffer + gapBuffer->gap_start,
    gapBuffer->size - gapBuffer->gap_start);
    gapBuffer->size = newSize;
}

void insertText(GapBuffer* gapBuffer, const char* text, int length) 
{
    // Check if there is enough space in the gap
    while (gapBuffer->gap_end - gapBuffer->gap_start + 1 < length) {
        resizeGapBuffer(gapBuffer);
    }

    // Move the content after the gap to make space for the new text
    memmove(gapBuffer->buffer + gapBuffer->gap_start + length,
            gapBuffer->buffer + gapBuffer->gap_start,
            gapBuffer->size - gapBuffer->gap_start);

    // Copy the new text into the gap
    memcpy(gapBuffer->buffer + gapBuffer->gap_start, text, length);

    // Update the gap_start to reflect the inserted text
    gapBuffer->gap_start += length;

    // Update the size of the buffer
    gapBuffer->size += length;

    // Update gap_end to maintain the gap
    gapBuffer->gap_end = gapBuffer->size - 1;

    // Null-terminate the buffer
    gapBuffer->buffer[gapBuffer->size] = '\0';
}

int findText(const GapBuffer *gapBuffer, const char *find)
{
    // Check for NULL pointers
    if (gapBuffer == NULL || find == NULL)
    {
        printf("Invalid input, NULL pointer\n");
        return -1;
    }

    // Check for empty strings
    if (gapBuffer->buffer[0] == '\0' || find[0] == '\0')
    {
        printf("Empty string\n");
        return -1;
    }

    // Check if find is within the valid range
    if (strlen(find) > gapBuffer->size)
    {
        printf("Substring length exceeds buffer size\n");
        return -1;
    }

    char *position = strstr(gapBuffer->buffer, find);
    if (position != NULL)
    {
        return position - gapBuffer->buffer;
    }
    return -1;
}

void copyText(GapBuffer *gapBuffer, int start, int length, char *destination)
{
    int i, j;

    // Check if start is within the valid range
    if (start < 0 || start >= gapBuffer->size) {
        printf("Invalid start index\n");
        return;
    }

    
    for (i = start, j = 0; j < length; ++i, ++j)
    {
        destination[j] = gapBuffer->buffer[i];
    }
    destination[j] = '\0'; // Null-terminate the copied text
}

void printBuffer(const GapBuffer* gapBuffer) 
{
    for (int i = 0; i < gapBuffer->size; ++i) 
    {
    if (i == gapBuffer->gap_start) {
    i = gapBuffer->gap_end;
    continue;
    }
    //if(i>=gapBuffer->gap_start && i<=gapBuffer->gap_end)
    //{printf("_");}
    else
    printf("%c", gapBuffer->buffer[i]);
    }
    printf("\n");
}

void freeGapBuffer(GapBuffer* gapBuffer) 
{
    free(gapBuffer->buffer);
}

void deleteText(GapBuffer* gapBuffer, int start, int length) 
{
    if (start < 0 || start + length > gapBuffer->size || length < 0)
    {
        printf("Invalid deletion range\n");
        return;
    }
    length-=1;      //delete char from 2 to 6 inclusive of both
    memmove(gapBuffer->buffer + start,
            gapBuffer->buffer + start + length,
            gapBuffer->size - (start + length));

    // Adjust the gap_start and size
    gapBuffer->gap_start -= length;
    gapBuffer->size -= length;

    // Adjust the gap_end to maintain the gap
    gapBuffer->gap_end -= length;

    printf("Deletion is DONE\n");
}

void grow(GapBuffer *gapBuffer, int k, int position) 
{
    char *a = (char *)malloc((gapBuffer->size - position) * sizeof(char));
    for (int i = position; i < gapBuffer->size; i++) {
        a[i - position] = gapBuffer->buffer[i];
    }

    gapBuffer->buffer = (char *)realloc(gapBuffer->buffer, (gapBuffer->size + k) * sizeof(char));

    if (gapBuffer->buffer == NULL) {
        // Handle realloc failure, print an error message, and exit or return
        fprintf(stderr, "Error: Unable to reallocate memory.\n");
        exit(EXIT_FAILURE);  // or return an error code
    }

    for (int i = 0; i < k; i++) {
        gapBuffer->buffer[i + position] = '_';
    }

    for (int i = 0; i < position + k; i++) {
        gapBuffer->buffer[position + k + i] = a[i];
    }

    free(a);

    gapBuffer->gap_start += k;
    gapBuffer->gap_end += k;
    gapBuffer->size += k;
    printf("After growing: ");
    printBuffer(gapBuffer);
    printf("\n");
}

void left(GapBuffer *gapBuffer, int position) 
{
    while (position < gapBuffer->gap_start) {
        if (gapBuffer->gap_start <= 0) {
            // Ensure the cursor doesn't go beyond the beginning of the buffer
            printf("Cursor reached the beginning of the buffer.\n");
            break;
        }

        gapBuffer->gap_start--;
        gapBuffer->gap_end--;

        // Move the character from the left of the cursor to the right
        gapBuffer->buffer[gapBuffer->gap_end + 1] = gapBuffer->buffer[gapBuffer->gap_start];
    }

    printf("After move cursor left: ");
    printBuffer(gapBuffer);
    printf("\n");
}

void right(GapBuffer *gapBuffer, int position) 
{
    while (position > gapBuffer->gap_start) {
        if (gapBuffer->gap_end >= gapBuffer->size - 1) {
            // Ensure the cursor doesn't go beyond the end of the buffer
            printf("Cursor reached the end of the buffer.\n");
            break;
        }

        gapBuffer->gap_start++;
        gapBuffer->gap_end++;

        // Move the character from the right of the cursor to the left
        gapBuffer->buffer[gapBuffer->gap_start - 1] = gapBuffer->buffer[gapBuffer->gap_end];
    }

    printf("After move cursor right: ");
    printBuffer(gapBuffer);
    printf("\n");
}

void move_cursor(GapBuffer *gapBuffer, int position) 
{
    if (position < 0 || position > gapBuffer->size) {
        printf("Invalid cursor position: %d\n", position);
        return;
    }

    if (position < gapBuffer->gap_start) {
        left(gapBuffer, position);
    } else {
        right(gapBuffer, position);
    }
}

void pasteText(GapBuffer *gapBuffer, const char *destination, int position)
{
    int len = strlen(destination);
    int i = 0;

    if (position < 0 || position > gapBuffer->size - 1)
    {
        printf("Invalid insertion position\n");
        return;
    }

    if (position != gapBuffer->gap_start)
    {
        move_cursor(gapBuffer, position);
    }

    int gap_size = gapBuffer->gap_end - gapBuffer->gap_start + 1;

    if (len > gap_size)
    {
        // If the gap is not large enough, grow the size
        int k = len - gap_size;
        int newSize = gapBuffer->size + k;
        grow(gapBuffer,k,position);

        // Shift the existing content to accommodate the new text
        for (int j = gapBuffer->size - 1; j >= gapBuffer->gap_end; j--)
        {
            gapBuffer->buffer[j + k] = gapBuffer->buffer[j];
        }

    }

    while (i < len)
    {
        gapBuffer->buffer[gapBuffer->gap_start] = destination[i];
        gapBuffer->gap_start++;
        i++;
        position++;
    }
    printf("After pasting: ");
    printBuffer(gapBuffer);
}

void cutText(GapBuffer *gapBuffer, int start, int end, char* destination)
{
    if (start >= end || end >= gapBuffer->size || destination == NULL)
    {
        printf("Can't cut text\n");
        return;
    }

    if (start != gapBuffer->gap_end + 1)
    {
        move_cursor(gapBuffer, end+1);
    }

    int count = end - start + 1;
    gapBuffer->gap_start = start;

    // Copy the cut characters to the destination array
    for (int i = 0; i < count; i++)
    {
        destination[i] = gapBuffer->buffer[gapBuffer->gap_start + i];
        gapBuffer->buffer[gapBuffer->gap_start + i] = '_';
    }

    // Null-terminate the destination array
    destination[count] = '\0';
    printf("Destination: %s\n",destination);
    printf("After cutting: ");
    printBuffer(gapBuffer);
}

void findAndReplace(GapBuffer* gapBuffer, const char* find, const char* replace) {
    int findLength = strlen(find);
    int replaceLength = strlen(replace);

    int position = findText(gapBuffer, find);
    // Check if the find string is present in the buffer
    if (position != -1) 
    {
        // Cut the found text
        char cuttext[findLength + 1];
        cutText(gapBuffer, position, position + findLength, cuttext);

        // Insert the replacement text
        pasteText(gapBuffer,replace,position);

    } else 
    {
        // Print a message if the find string is not present
        printf("Text not found: %s\n", find);
    }
}

int main()
{
    GapBuffer myGapBuffer;
    int bufferSize = 20;
    initGapBuffer(&myGapBuffer, bufferSize);
    int choice, position, start, end;
    char text[100], replace[100], destination[100];

    do
    {
        printf("1.insertText\n2.find\n3.find and replace\n4.copy Text\n5.cut Text\n6.paste text\n7.print gap buffer\n8.delete Text\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter text to be inserted: ");
            scanf(" %99[^\n]", text); // Using %99[^\n] to avoid buffer overflow
            insertText(&myGapBuffer, text, strlen(text));
            break;
        case 2: {
                    char find[100];
                    printf("Enter the text to be found: ");
                    scanf(" %99[^\n]", find); 
                    int res=findText(&myGapBuffer,find);
                    if(res==-1)
                        printf("Not found\n");
                    else
                        printf("Found text\n");
                    break;
        }

        case 3: {
                    char find[100], replace[100];
            printf("Enter text to be found: ");
            scanf(" %99[^\n]", find); // Using %99[^\n] to avoid buffer overflow
            printf("Enter text to replace it with: ");
            scanf(" %99[^\n]", replace); // Using %99[^\n] to avoid buffer overflow
            findAndReplace(&myGapBuffer, find, replace);
            break;}

        case 4:
            printf("Enter start and end index to copy text: ");
            scanf("%d%d", &start, &end);
            copyText(&myGapBuffer, start, end, destination);
            printf("Destination: %s\n",destination);
            break;

        case 5:
            printf("Enter cut start and end positions: ");
            scanf("%d %d", &start, &end);
            cutText(&myGapBuffer, start, end, destination);
            printf("Destination: %s\n",destination);
            break;

        case 6:
            printf("Enter insert position: ");
            scanf("%d", &position);
            pasteText(&myGapBuffer, destination, position);
            break;

        case 7:
            printBuffer(&myGapBuffer);
            break;

        case 8:
            printf("Enter cut start and end positions: ");
            scanf("%d %d", &start, &end);
            deleteText(&myGapBuffer, start, end);
            break;

        default:
            printf("Invalid choice\n");
            break;
        }

    } while (choice < 9);

    freeGapBuffer(&myGapBuffer);
    return 0;
}