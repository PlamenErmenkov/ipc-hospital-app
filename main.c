#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>

#define MAX_NAME_LEN 100
#define DIAGNOSIS_LEN 100
#define PROGNOSIS_LEN 50
#define MSG_SIZE sizeof(Patient) - sizeof(long)

typedef struct
{
    long msg_type;
    char name[MAX_NAME_LEN];
    int age;
    char egn[11];
    char diagnosis[DIAGNOSIS_LEN];
    char prognosis[PROGNOSIS_LEN];
} Patient;

#define REGISTRY_QUEUE 1234
#define UNDERWORLD_QUEUE 5678

void registry()
{
    int msgid = msgget(REGISTRY_QUEUE, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }

    Patient patient;
    patient.msg_type = 1;
    strcpy(patient.name, "Иван Иванов");
    patient.age = 55;
    strcpy(patient.egn, "9999999999");
    strcpy(patient.diagnosis, "Грип");
    strcpy(patient.prognosis, "Ще го бъде");

    msgsnd(msgid, &patient, MSG_SIZE, 0);
    printf("[РЕГИСТРАТУРА] Добавен пациент: %s, диагноза: %s, прогноза: %s\n",
           patient.name, patient.diagnosis, patient.prognosis);
}

void chief_doctor()
{
    int msgid = msgget(REGISTRY_QUEUE, 0666);
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }

    Patient patient;
    msgrcv(msgid, &patient, MSG_SIZE, 1, 0);
    printf("[ГЛАВЕН ЛЕКАР] Преглед на %s, диагноза: %s\n", patient.name, patient.diagnosis);

    if (rand() % 2 == 0)
    {
        strcpy(patient.prognosis, "Няма да го бъде");
        printf("[ГЛАВЕН ЛЕКАР] Лоша прогноза за %s\n", patient.name);

        int underworld_msgid = msgget(UNDERWORLD_QUEUE, 0666 | IPC_CREAT);
        msgsnd(underworld_msgid, &patient, MSG_SIZE, 0);
    }
    else
    {
        printf("[ГЛАВЕН ЛЕКАР] Добра прогноза за %s\n", patient.name);
    }
}

void underworld()
{
    int msgid = msgget(UNDERWORLD_QUEUE, 0666);
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }

    Patient patient;
    msgrcv(msgid, &patient, MSG_SIZE, 1, 0);
    printf("[ПОДЗЕМИЯТА] Приехме пациент: %s с лоша прогноза.\n", patient.name);
}

int main()
{
    srand(time(NULL));

    if (fork() == 0)
    {
        registry();
        exit(0);
    }
    sleep(1);

    if (fork() == 0)
    {
        chief_doctor();
        exit(0);
    }
    sleep(1);

    if (fork() == 0)
    {
        underworld();
        exit(0);
    }

    sleep(2);
    msgctl(REGISTRY_QUEUE, IPC_RMID, NULL);
    msgctl(UNDERWORLD_QUEUE, IPC_RMID, NULL);
    return 0;
}
