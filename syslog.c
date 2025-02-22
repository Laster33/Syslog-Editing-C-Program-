#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// LogEntry yap�s�: Her bir log kayd�n� temsil eder
struct LogEntry {
    char timestamp[32];
    char hostname[32];
    char process[64];
    char message[256];
    struct LogEntry *next;
};

// Yeni bir log kayd�n� ba�l� listenin sonuna ekler
void addLogEntry(struct LogEntry **head, char *timestamp, char *hostname, char *process, char *message) {
    struct LogEntry *newEntry = (struct LogEntry *)malloc(sizeof(struct LogEntry));
    strcpy(newEntry->timestamp, timestamp);
    strcpy(newEntry->hostname, hostname);
    strcpy(newEntry->process, process);
    strcpy(newEntry->message, message);
    newEntry->next = NULL;

    // E�er liste bo�sa, yeni kayd� ba� olarak ayarla
    if (*head == NULL) {
        *head = newEntry;
    } else {
        // Listenin sonuna git
        struct LogEntry *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Yeni kayd� listenin sonuna ekle
        current->next = newEntry;
    }
}

// T�m log kay�tlar�n� ekrana yazd�r�r
void printLogEntries(struct LogEntry *head) {
    struct LogEntry *current = head;
    while (current != NULL) {
        printf("Zaman Damgasi: %s\n", current->timestamp);
        printf("Hostname: %s\n", current->hostname);
        printf("Proses: %s\n", current->process);
        printf("Mesaj: %s\n", current->message);
        printf("----------------------------\n");
        current = current->next;
    }
}

// Belirli bir indeksteki log kayd�n� g�sterir
void showSpecificEntry(struct LogEntry *head) {
    int index;
    printf("Gostermek istediginiz kaydin indeksini girin: ");
    scanf("%d", &index);

    struct LogEntry *current = head;
    int count = 0;

    // Belirtilen indekse kadar listeyi gez
    while (current != NULL) {
        if (count == index) {
            printf("\nKayit %d:\n", index);
            printf("Zaman Damgasi: %s\n", current->timestamp);
            printf("Hostname: %s\n", current->hostname);
            printf("Proses: %s\n", current->process);
            printf("Mesaj: %s\n", current->message);
            printf("----------------------------\n");
            return;
        }
        current = current->next;
        count++;
    }

    // E�er indeks ge�ersizse hata mesaj� g�ster
    printf("Hata: %d indeksine sahip bir kayit bulunamadi.\n", index);
}

// Belirli bir indeksteki log kayd�n� siler
void deleteSpecificEntry(struct LogEntry **head) {
    int index;
    printf("Silmek istediginiz kaydin indeksini girin: ");
    scanf("%d", &index);

    struct LogEntry *current = *head;
    struct LogEntry *previous = NULL;
    int count = 0;

    // Belirtilen indekse kadar listeyi gez
    while (current != NULL) {
        if (count == index) {
            // E�er silinecek kay�t ba�lang��taysa
            if (previous == NULL) {
                *head = current->next;
            } else {
                // �nceki kayd�n next'ini g�ncelle
                previous->next = current->next;
            }

            // Kayd� bellekten sil
            free(current);
            printf("Kayit %d basariyla silindi.\n", index);
            return;
        }

        // Bir sonraki kayda ge�
        previous = current;
        current = current->next;
        count++;
    }

    // E�er indeks ge�ersizse hata mesaj� g�ster
    printf("Hata: %d indeksine sahip bir kayit bulunamadi.\n", index);
}

// Ba�l� listedeki log kay�tlar�n� yeni bir dosyaya kaydeder
void saveToNewFile(struct LogEntry *head) {
    char filename[256];
    printf("Yeni dosyanin adini girin: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Hata: Dosya olusturulamadi.\n");
        return;
    }

    struct LogEntry *current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %s %s\n", current->timestamp, current->hostname, current->process, current->message);
        current = current->next;
    }

    fclose(file);
    printf("Kayitlar basariyla '%s' dosyasina kaydedildi.\n", filename);
}

int main() {
    FILE *file = fopen("syslog", "r");
    if (file == NULL) {
        perror("Dosya acilirken hata olustu");
        return 1;
    }

    struct LogEntry *head = NULL;
    char line[512];

    // Dosyadan loglar� oku ve ba�l� listeye ekle
    while (fgets(line, sizeof(line), file)) {
        char timestamp[32], hostname[32], process[64], message[256];
        sscanf(line, "%s %s %s %[^\n]", timestamp, hostname, process, message);
        addLogEntry(&head, timestamp, hostname, process, message);
    }

    fclose(file);

    // Kullan�c�ya men�y� g�ster ve se�im yapmas�n� iste
    int choice;
    do {
        printf("\nLutfen bir islem secin:\n");
        printf("1. Tum kayitlari goster\n");
        printf("2. Belirli bir kaydi goster\n");
        printf("3. Belirli bir kaydi sil\n");
        printf("4. Kayitlari yeni bir dosyaya kaydet\n");
        printf("5. Programdan cik\n");
        printf("Seciminiz: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nTum kayitlar:\n");
                printLogEntries(head);
                break;
            case 2:
                showSpecificEntry(head);
                break;
            case 3:
                deleteSpecificEntry(&head);
                break;
            case 4:
                saveToNewFile(head);
                break;
            case 5:
                printf("Programdan cikiliyor...\n");
                break;
            default:
                printf("Gecersiz secim! Lutfen 1 ile 5 arasinda bir sayi girin.\n");
                break;
        }
    } while (choice != 5);

    // Belle�i temizle
    struct LogEntry *current = head;
    while (current != NULL) {
        struct LogEntry *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
