
void ArgumentParser(int argc, char** argv, int* workerAmount,int* workerSimLimit, int* workerArgument);

int ValidateInput(int workerAmount, int workerSimLimit, int workerArgument);

void WorkerHandler(int workerAmount, int workerSimLimit, int workerArgument);

int AwaitWorker();

void WorkerLauncher(int limit, int workerArgument);

void Help();
