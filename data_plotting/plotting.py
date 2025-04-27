# File to help parse and plot input data

import matplotlib.pyplot as plt


#data = [0, 0.71,0.79,1.29,1.50,1.72,1.86,2.14,2.14,2.43,2.64,2.86,3.29,3.29,3.65,3.79,3.93,4.29,4.43,4.79,4.79,4.79,4.79,4.79,4.79,4.43,4.50,4.65,4.79,4.86,5.15,5.22,5.29,5.29,5.43,5.43,5.43,5.43,5.43,5.43,5.43]

#force = [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80]

#start = [0,80]
#end = [0, data[-1]] 

#stiff_co = 80/(data[-1]/10)
#print(stiff_co)

ball_num = range(1, 21)
bounce_test = [54, 49, 55, 54, 55, 45, 54.5, 55, 57, 55, 51.5, 58, 54, 54, 55, 57, 60, 52, 54, 53.5, 51]
stiffness_test = [9.51,9.51,8.22,9.36,9.15,8,7.58,8.93,8.58,10.15,8.15,9.22,8.43,6.72,9.15,8.86,9.58,8.36,7.86,7.50,7.79]

disp_upper_bound = [108.1081081]*30
disp_lower_bound = [142.8571429]*30

bounce_lower_bound = [53] * 80
bounce_upper_bound = [58] * 80


for ball in range(len(stiffness_test)):
   stiffness_test[ball] = 80/(stiffness_test[ball]/10)

print(stiffness_test)


plt.plot(bounce_test, stiffness_test, "o", color = 'b')
plt.xlim(50,63)
plt.ylim(75,150)
plt.plot(range(35, 65), disp_lower_bound, 'b', linestyle='dashed')
plt.plot(range(35, 65), disp_upper_bound, 'b', linestyle='dashed')
plt.plot(bounce_lower_bound, range(75, 155), 'r', linestyle='dashed')
plt.plot(bounce_upper_bound, range(75, 155), 'r', linestyle='dashed')
plt.fill_between([53,58], [108.1081081], [142.8571429], color='green', alpha=0.3, label='Competitive')
plt.annotate("ITF Max Stiffness", xy=(63, 142.8571429), xytext=(63.2, 110), fontsize=10)
plt.annotate('ITF Min Stiffness', xy=(63, 108.1081081), xytext=(63.2, 144), fontsize=10)
plt.annotate('ITF Max Bounce Height', xy=(58, 150), xytext=(58.3, 147), fontsize=10)
plt.annotate('ITF Min Bounce Height', xy=(53, 150), xytext=(53.3, 147), fontsize=10)
plt.legend()
plt.xlabel("Bounce Height Test (inches)")
plt.ylabel("Stiffness Test (N/cm)")
plt.title("Evaluating Consistency Between Bounce and Stiffness Metrics")
plt.grid(color='lightgray', linestyle='--', linewidth=0.5, alpha=0.6)
plt.show()


