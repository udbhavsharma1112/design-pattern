#include<bits/stdc++.h>
#define int long long
using namespace std;

//decorator pattern
//design pizza with different toppings that calculates cost of the final pizza

class iBasePizza{
	public:
	virtual int cost() = 0;
};

class cornPizza: public iBasePizza{
	int price;
	public:
	cornPizza(int price) {
		this->price = price;
	}
	int cost(){
		return price;
	}
};

class farmHouse: public iBasePizza{
	int price;
	public:
	farmHouse(int price){
		this->price = price;
	}
	int cost(){
		return price;
	}
};

class extraCheese: public iBasePizza{
	int extraPrice;
	iBasePizza *basePizza;
	public:
	extraCheese(int extraPrice, iBasePizza* pizza) {
		this->extraPrice = extraPrice;
		this->basePizza = pizza;
	}
	int cost(){
		return basePizza->cost()+extraPrice;
	}
};

class extraPaneer: public iBasePizza{
	int extraPrice;
	iBasePizza *basePizza;
	public:
	extraPaneer(int extraPrice, iBasePizza* pizza) {
		this->extraPrice = extraPrice;
		this->basePizza = pizza;
	}
	int cost(){
		return basePizza->cost()+extraPrice;
	}
};

int32_t main()
{
	iBasePizza *farmHousePizza = new farmHouse(400);
	iBasePizza *pizzaWithExtraCheese  = new extraCheese(80,farmHousePizza);
	cout<<pizzaWithExtraCheese->cost()<<endl;
	iBasePizza * pizzaWithExtraCheeseAndExtraPaneer = new extraPaneer(80,pizzaWithExtraCheese);
	cout<<pizzaWithExtraCheeseAndExtraPaneer->cost()<<endl;
	
}

