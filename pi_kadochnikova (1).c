Last login: Mon Sep 14 15:41:24 on ttys008

The default interactive shell is now zsh.
To update your account to use zsh, please run `chsh -s /bin/zsh`.
For more details, please visit https://support.apple.com/kb/HT208050.
(base) MacBook-Pro-Olga:~ olgakadocnikova$ ssh -p 52960 bm202006@remote.vdi.mipt.ru
bm202006@remote.vdi.mipt.ru's password: 
Last login: Mon Sep 14 11:49:13 2020 from 10.55.190.242
[bm202006@head ~]$ touch pi.c
[bm202006@head ~]$ vi pi.c



[bm202006@head ~]$ gcc pi.ex
gcc: ошибка: pi.ex: Нет такого файла или каталога
gcc: фатальная ошибка: не заданы входные файлы
компиляция прервана.
[bm202006@head ~]$ gcc pi.c
pi.c:1:8: ошибка: expected «=», «,», «;», «asm» or «__attribute__» before «<» token
 nclude <stdio.h>
        ^
pi.c: В функции «main»:
pi.c:11:5: предупреждение: несовместимая неявная декларация внутренней функции scanf» [по умолчанию включена]
     scanf("%d", &N);
     ^
pi.c:23:5: предупреждение: несовместимая неявная декларация внутренней функции printf» [по умолчанию включена]
     printf("%f\n", S);
     ^
[bm202006@head ~]$ vi pi.c
[bm202006@head ~]$ gcc pi.c
/tmp/ccyq7v2B.o: In function `f':
pi.c:(.text+0x28): undefined reference to `sqrt'
collect2: ошибка: выполнение ld завершилось с кодом возврата 1
[bm202006@head ~]$ ./pi.c
-bash: ./pi.c: Отказано в доступе
[bm202006@head ~]$ vi pi.c
[bm202006@head ~]$ gcc pi.c
/tmp/ccQsczkr.o: In function `f':
pi.c:(.text+0x28): undefined reference to `sqrt'
collect2: ошибка: выполнение ld завершилось с кодом возврата 1
[bm202006@head ~]$ gcc pi.c -o pi -lm
[bm202006@head ~]$ ./pi.c
-bash: ./pi.c: Отказано в доступе
[bm202006@head ~]$ gcc pi.c -o mysqrt -lm
[bm202006@head ~]$ ./pi.c
-bash: ./pi.c: Отказано в доступе
[bm202006@head ~]$ gcc mysqrt.c -o mysqrt -lm
gcc: ошибка: mysqrt.c: Нет такого файла или каталога
[bm202006@head ~]$ gcc pi.c -o mysqrt -lm
[bm202006@head ~]$ ./pi.c
-bash: ./pi.c: Отказано в доступе
[bm202006@head ~]$ ./a.out
-bash: ./a.out: Нет такого файла или каталога
[bm202006@head ~]$ ./mysqrt
1000
0.000000
[bm202006@head ~]$ ./mysqrt
40000
3.141592
[bm202006@head ~]$ ./mysqrt
435452
3.141593
[bm202006@head ~]$ ./mysqrt
45683
3.141592
[bm202006@head ~]$ ./mysqrt
43              
3.127453
[bm202006@head ~]$ vi pi.c

#include <stdio.h>
#include <math.h>

double f(double x) {
    return sqrt((4 - x * x));
}

int main() {
    double h, x, x_i, S, now_S;
    int N;
    scanf("%d", &N);

    h = 2.0 / N;
    x = 0;
    x_i = h;

    while (x_i < 2) {
        now_S = (h / 2) * (f(x) + f(x_i));
        S += now_S;
        x = x_i;
        x_i += h;
    }
    printf("%f\n", S);
    return 0;
}
