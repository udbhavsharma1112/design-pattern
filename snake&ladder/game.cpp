#include<bits/stdc++.h>
using namespace std;

class Dice{
    int range;
    public:
    Dice():range(6){}
    Dice(int range){
        this->range = range;
    }
    int getDiceValue(){
        srand(time(0));
        return rand()%range+1;
    }
};
class User{
    int playerId;
    string playerName;
    public:
        User(): playerId(-1),playerName(){};
        User(int playerId, string playerName){
            this->playerId=playerId;
            this->playerName=playerName;
        }
        string getPlayerName(){
            return this->playerName;
        }

};
class Player:public User{
        int currentStep;
        Dice dice;
        bool winner;
    public:
        Player():currentStep(0),dice(),winner(0){}
        Player(int currentStep, int playerId, string playerName): User(playerId, playerName), currentStep(currentStep), dice(6), winner(false) {}
        int play(){
            return dice.getDiceValue();
        }
        void updateCurrentStep(int currentStep){
            this->currentStep = currentStep;
        }
        void setWinnerStatus(){
            this->winner=1;
        }
        bool getWinnerStatus(){
            return this->winner;
        }
        int getCurrentStep(){
            return this->currentStep;
        }
};


class Snake{
    map<int,int>snakeMapping;
    public:
    Snake():snakeMapping(){}
    Snake(map<int,int> snakeMapping){
        this->snakeMapping=snakeMapping;
    }
    int getDestination(int x){
        if(snakeMapping.find(x)==snakeMapping.end()){
            return x;
        }
        return snakeMapping[x];
    }
};
class Ladder{
    map<int,int>ladderMapping;
    public:
    Ladder():ladderMapping(){}
    Ladder(map<int,int>ladderMapping){
        this->ladderMapping=ladderMapping;
    }
    int getDestination(int x){
        if(ladderMapping.find(x)==ladderMapping.end()){
            return x;
        }
        return ladderMapping[x];
    }
};

class Board{
    int maxNumber;
    Snake snake;
    Ladder ladder;
    public:
    Board():maxNumber(100),snake(),ladder(){}
    Board(int maxNumber, Snake snake, Ladder ladder){
        this->maxNumber=maxNumber;
        this->snake=snake;
        this->ladder=ladder;
    }
    bool isWinner(int currentStep){
        if(currentStep==maxNumber) return 1;
        return 0;
    }
    int getUpdatedMove(int currentStep, int step){
        if(currentStep+step>maxNumber) {
            return currentStep;
        }
        else{
            int tempDestination = currentStep + step;
            int x = snake.getDestination(tempDestination);
            if(x==tempDestination){
                x = ladder.getDestination(tempDestination);
            }
            return x;
        }
    }
};


class gameManager{
    Board board;
    vector<Player>players;
    public:
    gameManager(): board(),players(){}
    gameManager(Board board){
        this->board = board;
    }
    void addPlayer(Player player){
        this->players.push_back(player);
    }
    void start(){
        int end=0;
        while(end<players.size()-1  ){
            for(auto &player:players){
            if(player.getWinnerStatus()){
                continue;
            }
            cout<<player.getPlayerName()<<": write PLAY to throw a Dice"<<endl;
            string play_text; cin>>play_text;
            int x = player.play();
            cout<<"you got "<<x<<endl;
            int prevStep = player.getCurrentStep();
            int currentStep = board.getUpdatedMove(prevStep,x);
            if(currentStep<prevStep+x){
                cout<<"OPPS, you are at number "<<currentStep<<endl;
            }else if(currentStep==prevStep+x){
                cout<<"You reached at number "<<currentStep<<endl;
            }else if(currentStep>prevStep+x){
                cout<<"Hurry!!, you reached at number "<<currentStep<<endl;
            }
            player.updateCurrentStep(currentStep);
            if(this->board.isWinner(currentStep)){
                cout<<"Congrats!!!!!!! "<<player.getPlayerName()<<", you won "<<endl;
                end++;
                player.setWinnerStatus();
            }
            }
        }
    }



};

int main(){
    map<int,int>snakeMapping;
    snakeMapping[27]=7;
    snakeMapping[45]=9;
    snakeMapping[32]=12;
    snakeMapping[52]=19;
    snakeMapping[67]=31;
    snakeMapping[71]=50;
    snakeMapping[89]=67;
    snakeMapping[98]=30;
    map<int,int>ladderMapping;
    ladderMapping[9]=39;
    ladderMapping[20]=41;
    ladderMapping[41]=61;
    ladderMapping[50]=82;
    ladderMapping[64]=91;
    ladderMapping[32]=62;

    Snake snake = Snake(snakeMapping);
    Ladder ladder = Ladder(ladderMapping);
    Board board = Board(100, snake, ladder);
    gameManager game = gameManager(board);
    Player player1 = Player(0,1,"Udbhav");
    Player player2 = Player(0,2,"Dora");
    Player player3 = Player(0,3,"Murarka");
    Player player4 = Player(0,4,"Utkarsh");
    game.addPlayer(player1);
    game.addPlayer(player2);
    game.addPlayer(player3);
    game.addPlayer(player4);
    game.start();

}