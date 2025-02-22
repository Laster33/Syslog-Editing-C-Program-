#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// LogEntry yapýsý: Her bir log kaydýný temsil eder
struct LogEntry {
    char timestamp[32];
    char hostname[32];
    char process[64];
    char message[256];
    struct LogEntry *next;
};

// Yeni bir log kaydýný baðlý listenin sonuna ekler
void addLogEntry(struct LogEntry **head, char *timestamp, char *hostname, char *process, char *message) {
    struct LogEntry *newEntry = (struct LogEntry *)malloc(sizeof(struct LogEntry));
    strcpy(newEntry->timestamp, timestamp);
    strcpy(newEntry->hostname, hostname);
    strcpy(newEntry->process, process);
    strcpy(newEntry->message, message);
    newEntry->next = NULL;

    // Eðer liste boþsa, yeni kaydý baþ olarak ayarla
    if (*head == NULL) {
        *head = newEntry;
    } else {
        // Listenin sonuna git
        struct LogEntry *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Yeni kaydý listenin sonuna ekle
        current->next = newEntry;
    }
}

// Tüm log kayýtlarýný ekrana yazdýrýr
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

// Belirli bir indeksteki log kaydýný gösterir
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

    // Eðer indeks geçersizse hata mesajý göster
    printf("Hata: %d indeksine sahip bir kayit bulunamadi.\n", index);
}

// Belirli bir indeksteki log kaydýný siler
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
            // Eðer silinecek kayýt baþlangýçtaysa
            if (previous == NULL) {
                *head = current->next;
            } else {
                // Önceki kaydýn next'ini güncelle
                previous->next = current->next;
            }

            // Kaydý bellekten sil
            free(current);
            printf("Kayit %d basariyla silindi.\n", index);
            return;
        }

        // Bir sonraki kayda geç
        previous = current;
        current = current->next;
        count++;
    }

    // Eðer indeks geçersizse hata mesajý göster
    printf("Hata: %d indeksine sahip bir kayit bulunamadi.\n", index);
}

// Baðlý listedeki log kayýtlarýný yeni bir dosyaya kaydeder
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

    // Dosyadan loglarý oku ve baðlý listeye ekle
    while (fgets(line, sizeof(line), file)) {
        char timestamp[32], hostname[32], process[64], message[256];
        sscanf(line, "%s %s %s %[^\n]", timestamp, hostname, process, message);
        addLogEntry(&head, timestamp, hostname, process, message);
    }

    fclose(file);

    // Kullanýcýya menüyü göster ve seçim yapmasýný iste
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

    // Belleði temizle
    struct LogEntry *current = head;
    while (current != NULL) {
        struct LogEntry *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
