import numpy as np
import matplotlib.pyplot as plt
from sklearn import svm
import matplotlib
file = open("CSCI3230_SVM TrainingQ2.csv")
X = np.loadtxt(file, delimiter=",", skiprows=1, usecols=(0, 1))
file.seek(0)
y = np.loadtxt(file, delimiter=",", skiprows=1, usecols=2)
file.close()
clf = svm.SVC(kernel='poly', degree=2)
clf.fit(X, y)
#print("alpha:", clf.dual_coef_)
#print("w:", clf.coef_)
#print("support vectors: \n", clf.support_vectors_)

def plot_svc_decision_function(model, ax=None, plot_support=True):
    """Plot the decision function for a 2D SVC"""
    if ax is None:
        ax = plt.gca()
    xlim = ax.get_xlim()
    ylim = ax.get_ylim()
        # create grid to evaluate model\n",
    x = np.linspace(xlim[0], xlim[1], 30)
    y = np.linspace(ylim[0], ylim[1], 30)
    Y, X = np.meshgrid(y, x)
    xy = np.vstack([X.ravel(), Y.ravel()]).T
    P = model.decision_function(xy).reshape(X.shape)
        # plot decision boundary and margins\n",
    ax.contour(X, Y, P, colors='k',
               levels=[-1, 0, 1], alpha=0.5,
               linestyles=['--', '-', '--'])
    
        # plot support vectors\n",
    if plot_support:
        ax.scatter(model.support_vectors_[:, 0],
                    model.support_vectors_[:, 1],
                   s=100, linewidth=1, facecolors='none', edgecolors='k')
    ax.set_xlim(xlim)
    ax.set_ylim(ylim)

plt.scatter(X[:, 0], X[:, 1], c=y, s=50, cmap=plt.cm.Paired)
plot_svc_decision_function(clf, None, True)
plt.title("Q2 Training")
plt.show()

file = open("CSCI3230_SVM TestQ2.csv")
testX = np.loadtxt(file, delimiter=",", skiprows=1, usecols=(0, 1))
file.seek(0)
testy = np.loadtxt(file, delimiter=",", skiprows=1, usecols=2)
file.close()
plt.scatter(testX[:, 0], testX[:, 1], c=testy, s=50, cmap=plt.cm.Paired)
plot_svc_decision_function(clf, None, False)
plt.title("Q2 Test")
plt.show()
