g++ -std=c++11 signal-receiver.cpp -o signal-receiver
g++ -std=c++11 signal-sender.cpp   -o signal-sender
./signal-receiver

./signal-sender 12345 10
tail -f signals.log
