#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void catch_signal(int signum) {
    printf("Ujet signal %d\n", signum);
}

int main() {
    // Lovljenje signalov
    write(2, "Lovim signal SIGINT ...\n", 25);
    signal(SIGINT, catch_signal);
    int unslept = sleep(5); // Počakajte 5 sekund
    // Če je bil sleep prekinjen, ga pokličemo še enkrat
    while (unslept > 0) {
        printf("Unslept time: %d\n", unslept);
        unslept = sleep(unslept); // Počakajte 1 sekundo
    }
    write(2, "Alarm v času t = 5...\n", 23);

    // Ignoriranje signalov
    write(2, "Ignoriram signal SIGINT ...\n", 29);
    signal(SIGINT, SIG_IGN);
    sleep(5);
    write(2, "Alarm v času t = 10...\n", 24);

    // Privzeta akcija za signal
    write(2, "Privzeta akcija za SIGINT ...\n", 31);
    signal(SIGINT, SIG_DFL);
    sleep(5);
    write(2, "Alarm v času t = 15...\n", 24);

    return 0;
}
