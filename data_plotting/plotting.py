# File to help parse and plot input data

import matplotlib.pyplot as plt


#data = [0, 0.71,0.79,1.29,1.50,1.72,1.86,2.14,2.14,2.43,2.64,2.86,3.29,3.29,3.65,3.79,3.93,4.29,4.43,4.79,4.79,4.79,4.79,4.79,4.79,4.43,4.50,4.65,4.79,4.86,5.15,5.22,5.29,5.29,5.43,5.43,5.43,5.43,5.43,5.43,5.43]

#force = [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80]

#start = [0,80]
#end = [0, data[-1]] 

#stiff_co = 80/(data[-1]/10)
#print(stiff_co)

ball_num = range(1, 25)
bounce_test = [54, 56, 54, 55, 55, 54, 55, 56, 54, 56, 53, 52, 54, 52, 58, 59, 48, 55, 56, 50, 55, 56, 56, 53, 52]
stiffness_test = [6.73, 6.37, 6.94, 5.95, 5.6, 7.15, 6.52, 6.16, 7.01, 7.3, 6.16, 6.45, 7.09, 8.57, 6.94, 6.16, 9.43, 7.22, 6.66, 7.01, 6.45, 7.01, 7.35, 6.02, 9.03]

disp_upper_bound = [108.1081081]*30
disp_lower_bound = [142.8571429]*30

practice_disp_lower_bound = [108.1081081]*30
practice_disp_upper_bound = [86]*30

bounce_lower_bound = [53] * 80
bounce_upper_bound = [58] * 80

practice_bounce_lower_bound = [50] * 80
practice_bounce_upper_bound = [53] * 80


for ball in range(len(stiffness_test)):
   stiffness_test[ball] = 80/(stiffness_test[ball]/10)

print(stiffness_test)


plt.plot(bounce_test, stiffness_test, "o", color = 'b')
plt.xlim(47,63)
plt.ylim(75,150)
plt.plot(range(35, 65), disp_lower_bound, 'b', linestyle='dashed')
plt.plot(range(35, 65), disp_upper_bound, 'b', linestyle='dashed')
plt.plot(range(35, 65), practice_disp_lower_bound, 'g', linestyle='dashed')
plt.plot(range(35, 65), practice_disp_upper_bound, 'g', linestyle='dashed')
plt.plot(bounce_lower_bound, range(75, 155), 'r', linestyle='dashed')
plt.plot(bounce_upper_bound, range(75, 155), 'r', linestyle='dashed')
plt.plot(practice_bounce_lower_bound, range(75, 155), 'm', linestyle='dashed')
plt.plot(practice_bounce_upper_bound, range(75, 155), 'm', linestyle='dashed')
plt.fill_between([53,58], [108.1081081], [142.8571429], color='green', alpha=0.3, label='Competitive')
plt.fill_between([50,53], [86], [108.1081081], color='yellow', alpha=0.3, label='Practice')
plt.fill_between([45,50], [70], [86], color='red', alpha=0.3, label='Recycle')
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


