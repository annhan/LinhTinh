//khởi tạo các job

WorkScheduler *ledSche1;
WorkScheduler *ledSche2;
WorkScheduler *ledSche3;

WorkScheduler *servoSche1;
WorkScheduler *servoSche2;
WorkScheduler *servoSche3;

int jobID = 0;

void pushNextJobToQueue() {
	jobID = (++jobID) % 6;
}

void ledProcess1()
{

	if (jobID == 0)
	{
		pushNextJobToQueue();
		//chương trình của bạn

	}

}

void servoProcess1()
{

	if (jobID == 1)
	{
		pushNextJobToQueue();
		//chương trình của bạn
	}
}

void ledProcess2()
{

	if (jobID == 2)
	{
		pushNextJobToQueue();
		//chương trình của bạn

	}

}

void servoProcess2()
{

	if (jobID == 3)
	{
		pushNextJobToQueue();
		//chương trình của bạn
	}
}

void ledProcess3()
{

	if (jobID == 4)
	{
		jobID++;pushNextJobToQueue();
		//chương trình của bạn

	}

}

void servoProcess3()
{

	if (jobID == 5)
	{
		pushNextJobToQueue();
		//chương trình của bạn
	}
}

int main() {
	//Khởi gạo class timer (design pattern singleton)
	Timer::getInstance()->initialize();
	
	ledSche1 = new WorkScheduler(0, ledProcess1);
	ledSche2 = new WorkScheduler(0, ledProcess2);
	ledSche3 = new WorkScheduler(0, ledProcess3);
	
	servoSche1 = new WorkScheduler(0, servoProcess1);
	servoSche2 = new WorkScheduler(0, servoProcess2);
	servoSche3 = new WorkScheduler(0, servoProcess3);
}

void loop()
{
	//đầu hàm loop phải có để cập nhập thời điểm diễn ra việc kiểm tra lại các tiến trình
	Timer::getInstance()->update();

	servoSche1->update();
	servoSche2->update();
	servoSche3->update();
	ledSche1->update();
	ledSche2->update();
	ledSche3->update();


	//cuối hàm loop phải có để cập nhập lại THỜI ĐIỂM (thời điểm chứ ko phải thời gian nha, tuy tiếng Anh chúng đều là time) để cho lần xử lý sau
	Timer::getInstance()->resetTick();
	
}