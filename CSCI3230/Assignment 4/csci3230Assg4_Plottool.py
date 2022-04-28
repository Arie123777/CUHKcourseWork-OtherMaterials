import matplotlib.pyplot as plt
f = open("3230Assg1e.txt", "r")
a = f.read()
numbers = []
for word in a.split():
    if word.replace('.','', 1).isdigit():
        numbers.append(float(word))
    else:
        newword = word.replace('%', '', 1).replace(',', '', 1)
        if newword.replace('.','', 1).isdigit():
            numbers.append(float(newword))
TestAcc = []
TestLoss = []
TrainAcc = []
TrainLoss = []
cnt = []
for count, value in enumerate(numbers):
    if (count%5 == 1):
        TrainAcc.append(value)
    if (count%5 == 2):
        TrainLoss.append(value)
    if (count%5 == 3):
        TestAcc.append(value)
    if (count%5 == 4):
        TestLoss.append(value)
    if (count%5 == 0):
        cnt.append(count/5+1)
plt.plot(cnt, TrainAcc, label="Train")
plt.plot(cnt, TestAcc, label = "Test")
plt.legend()
plt.xlabel("Epochs")
plt.ylabel("Accurcy (%)")
plt.show()
print(cnt)

plt.plot(cnt, TrainLoss, label="Train")
plt.plot(cnt, TestLoss, label = "Test")
plt.legend()
plt.xlabel("Epochs")
plt.ylabel("Average Loss")
plt.show()