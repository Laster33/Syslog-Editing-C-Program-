# Syslog-Editing-C-Program-

Bu proje, sistem loglarını okuma, görüntüleme, silme ve yeni bir dosyaya kaydetme işlemlerini gerçekleştiren basit bir C programıdır. Program, bir bağlı liste (linked list) kullanarak log kayıtlarını yönetir.

## Özellikler

- **Log Kayıtlarını Okuma**: `syslog` dosyasından log kayıtlarını okur ve bağlı listeye ekler.
- **Tüm Kayıtları Görüntüleme**: Bağlı listedeki tüm log kayıtlarını ekrana yazdırır.
- **Belirli Bir Kaydı Görüntüleme**: Kullanıcının belirttiği indeksteki log kaydını görüntüler.
- **Belirli Bir Kaydı Silme**: Kullanıcının belirttiği indeksteki log kaydını siler.
- **Kayıtları Yeni Bir Dosyaya Kaydetme**: Bağlı listedeki tüm log kayıtlarını yeni bir dosyaya kaydeder.

## Nasıl Kullanılır?

1. **Kodu Derleme**:
   ```bash
   gcc -o log_manager log_manager.c

2. **Kodu Çalıştırma**:
   ```bash
   ./log_manager
