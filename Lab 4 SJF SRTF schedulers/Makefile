sjf: sjf.cpp
	@gcc sjf.cpp -lstdc++
	@./a.out process1.dat

srtf: srtf.cpp
	@gcc srtf.cpp -lstdc++
	@./a.out test3.dat

graphs: waitingtime.py turnaround.py throughput.py
	@python waitingtime.py
	@python turnaround.py
	@python throughput.py
