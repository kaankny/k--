import time
import timeit

# Toplam çalışma süresini ölçmek için başlangıç zamanı
program_start_time = time.time()

def isPrime(num: int) -> bool:
    if num <= 1:
        print("Gecersiz sayi girdiniz.")  # "Invalid number entered."
        return False
    if num <= 3:
        return True
    if num % 2 == 0 or num % 3 == 0:
        return False
    i = 5
    while i * i <= num:
        if num % i == 0 or num % (i + 2) == 0:
            return False
        i += 6
    return True

# isPrime fonksiyonunun çalışma süresini ölçmek için başlangıç zamanı
start_time = time.time()

# isPrime fonksiyonunu çağır ve sonucu al
isPrimeResult = isPrime(999999999999999999999999999999999999999999)

# isPrime fonksiyonunun bitiş zamanı
end_time = time.time()

# isPrime fonksiyonunun süresi (milisaniye cinsinden)
elapsed_time_ms = (end_time - start_time) * 1000

# Toplam program çalışma süresini ölçmek için bitiş zamanı
program_end_time = time.time()
total_program_time_ms = (program_end_time - program_start_time) * 1000

# Sonuçları yazdır
print(f"isPrime sonucu: {isPrimeResult}")
print(f"isPrime fonksiyonunun çalışma süresi: {elapsed_time_ms} ms")
print(f"Toplam program çalışma süresi: {total_program_time_ms} ms")
