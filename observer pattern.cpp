#include<bits/stdc++.h>
using namespace std;

// design the notify me 
// system design

// two entities
//  -	notification management
//  -  observer/user

class iObserver{
	public:
	virtual void update() = 0;
};
class iStockManagementSystem{
	public:
	virtual void add(iObserver*) = 0;
	virtual void remove(iObserver*) = 0;
	virtual void notify() = 0;
	virtual void addStock(int) = 0;
};



class stockManagementSystem: public iStockManagementSystem{
	vector<iObserver *>observerList;
	int stockCount = 0;
	public:
	void add(iObserver *obsr) override{
		observerList.push_back(obsr);
	}
	void remove(iObserver *obsr)override{
		return;
	}
	void notify() override {
		for(auto obsr: observerList) {
			obsr->update();
		}
	}
	void addStock(int newStock)override{
		if(stockCount==0) {
			notify();
		}
		stockCount+=newStock;
	}
	
};

class sendEmail:public iObserver{
	public:
	void update() {
		cout<<"Email has been sent"<<endl;
	}
};
class sentByMobile: public iObserver{
	public:
	void update() {
		cout<<"message has been sent"<<endl;
	}
};

int main(){
	iStockManagementSystem *iphone = new stockManagementSystem();
	iObserver *observer1 = new sendEmail();
	iObserver *observer2 = new sentByMobile();
	iObserver *observer3 = new sendEmail();
	iphone->add(observer1);
	iphone->add(observer2);
	iphone->add(observer3);
	iphone->addStock(10);
}
