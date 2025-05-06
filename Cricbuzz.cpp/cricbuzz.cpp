#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

enum Role { BATSMAN, BOWLER, ALLROUNDER };
enum MatchStatus { WIN, DRAW, LOSE, INPROGRESS };

class IPlayer {
public:
    int id;
    std::string name;
    std::string age;
    std::string gender;
    Role role;

    IPlayer(int id, const std::string& name, const std::string& age, const std::string& gender, Role role)
        : id(id), name(name), age(age), gender(gender), role(role) {}

    virtual void displayDetails() const = 0;
    virtual ~IPlayer() = default;
};

class Player : public IPlayer {
public:
    Player(int id, const std::string& name, const std::string& age, const std::string& gender, Role role)
        : IPlayer(id, name, age, gender, role) {}

    void displayDetails() const override {
        std::cout << "Player ID: " << id << ", Name: " << name << ", Role: " << role << std::endl;
    }
};

class ITeam {
public:
    std::vector<std::shared_ptr<IPlayer>> players;
    std::string name;
    std::string country;
    std::string coach;
    int score = 0;
    int wickets = 0;
    MatchStatus status = MatchStatus::INPROGRESS;

    ITeam(const std::string& name, const std::string& country, const std::string& coach)
        : name(name), country(country), coach(coach) {}

    virtual void getTeamDetails() const = 0;
    virtual void addPlayer(std::shared_ptr<IPlayer> player) = 0;
    virtual void removePlayer(int playerId) = 0;
    virtual void updateScore(int run) = 0;
    virtual void updateWicket() = 0;
    virtual void updateStatus(MatchStatus status) = 0;
    virtual int getScore() const = 0;
    virtual int getWickets() const = 0;
    virtual MatchStatus getStatus() const = 0;
    virtual ~ITeam() = default;

    std::shared_ptr<IPlayer> findPlayer(int playerId) const {
        for (const auto& player : players) {
            if (player->id == playerId) {
                return player;
            }
        }
        return nullptr;
    }
};

class BowlingTeam : public ITeam {
public:
    BowlingTeam(const std::string& name, const std::string& country, const std::string& coach)
        : ITeam(name, country, coach) {}

    void getTeamDetails() const override {
        std::cout << "Bowling Team: " << name << std::endl;
        std::cout << "Country: " << country << std::endl;
        std::cout << "Coach: " << coach << std::endl;
    }

    void addPlayer(std::shared_ptr<IPlayer> player) override {
        players.push_back(player);
    }

    void removePlayer(int playerId) override {
        players.erase(std::remove_if(players.begin(), players.end(),
                                     [playerId](const std::shared_ptr<IPlayer>& player) {
                                         return player->id == playerId;
                                     }),
                      players.end());
    }

    void updateWicket() override {
        wickets++;
    }

    void updateStatus(MatchStatus status) override {
        this->status = status;
    }

    int getWickets() const override { return wickets; }
    MatchStatus getStatus() const override { return status; }
    int getScore() const override { return score; }

    void updateScore(int run) override {
    }
};

class BattingTeam : public ITeam {
public:
    BattingTeam(const std::string& name, const std::string& country, const std::string& coach)
        : ITeam(name, country, coach) {}

    void getTeamDetails() const override {
        std::cout << "Batting Team: " << name << std::endl;
        std::cout << "Country: " << country << std::endl;
        std::cout << "Coach: " << coach << std::endl;
    }

    void addPlayer(std::shared_ptr<IPlayer> player) override {
        players.push_back(player);
    }

    void removePlayer(int playerId) override {
        players.erase(std::remove_if(players.begin(), players.end(),
                                     [playerId](const std::shared_ptr<IPlayer>& player) {
                                         return player->id == playerId;
                                     }),
                      players.end());
    }

    void updateScore(int run) override {
        score += run;
    }

    void updateWicket() override {
        wickets++;
    }

    void updateStatus(MatchStatus status) override {
        this->status = status;
    }

    int getScore() const override { return score; }
    int getWickets() const override { return wickets; }
    MatchStatus getStatus() const override { return status; }
};

class Match {
private:
    std::shared_ptr<ITeam> team1;
    std::shared_ptr<ITeam> team2;
    int overs;
    int currentInning = 1;

public:
    Match(std::shared_ptr<ITeam> team1, std::shared_ptr<ITeam> team2, int overs)
        : team1(team1), team2(team2), overs(overs) {}

    void getMatchDetails() const {
        std::cout << "Team 1 (" << team1->name << "):" << std::endl;
        team1->getTeamDetails();
        std::cout << "\nTeam 2 (" << team2->name << "):" << std::endl;
        team2->getTeamDetails();
        std::cout << "Overs per innings: " << overs << std::endl;
        std::cout << "Current Inning: " << currentInning << std::endl;
    }

    void updateScore(int teamId, int runs) {
        if (teamId == 1) {
            team1->updateScore(runs);
        } else if (teamId == 2) {
            team2->updateScore(runs);
        } else {
            throw std::invalid_argument("Invalid team ID");
        }
    }

    void updateWicket(int teamId) {
        if (teamId == 1) {
            team1->updateWicket();
        } else if (teamId == 2) {
            team2->updateWicket();
        } else {
            throw std::invalid_argument("Invalid team ID");
        }
    }

    void updateStatus(MatchStatus status) {
        team1->updateStatus(status);
        team2->updateStatus(status);
    }

    void getScorecard() const {
        std::cout << "Match Scorecard:" << std::endl;
        std::cout << team1->name << ": " << team1->getScore() << "/" << team1->getWickets() << std::endl;
        std::cout << team2->name << ": " << team2->getScore() << "/" << team2->getWickets() << std::endl;
        std::cout << "Status: Team 1 - " << team1->getStatus() << ", Team 2 - " << team2->getStatus() << std::endl;
    }

    int getTeamScore(int teamId) const {
        if (teamId == 1) {
            return team1->getScore();
        } else if (teamId == 2) {
            return team2->getScore();
        } else {
            throw std::invalid_argument("Invalid team ID");
        }
    }

    int getTeamWickets(int teamId) const {
        if (teamId == 1) {
            return team1->getWickets();
        } else if (teamId == 2) {
            return team2->getWickets();
        } else {
            throw std::invalid_argument("Invalid team ID");
        }
    }

    MatchStatus getTeamStatus(int teamId) const {
        if (teamId == 1) {
            return team1->getStatus();
        } else if (teamId == 2) {
            return team2->getStatus();
        } else {
            throw std::invalid_argument("Invalid team ID");
        }
    }

    void setCurrentInning(int inning) {
        if (inning >= 1) {
            currentInning = inning;
        } else {
            throw std::invalid_argument("Inning number must be at least 1");
        }
    }

    int getCurrentInning() const {
        return currentInning;
    }
};

class Cricbuzz {
private:
    std::vector<std::shared_ptr<Match>> matches;

public:
    void addMatch(std::shared_ptr<Match> match) {
        matches.push_back(match);
    }

    void getMatches() const {
        for (const auto& match : matches) {
            match->getMatchDetails();
        }
    }

    std::shared_ptr<Match> getMatch(int index) const {
        if (index >= 0 && index < matches.size()) {
            return matches[index];
        } else {
            throw std::out_of_range("Invalid match index");
        }
    }

    void updateMatch(int index, std::shared_ptr<Match> match) {
        if (index >= 0 && index < matches.size()) {
            matches[index] = match;
        } else {
            throw std::out_of_range("Invalid match index");
        }
    }

    void removeMatch(int index) {
        if (index >= 0 && index < matches.size()) {
            matches.erase(matches.begin() + index);
        } else {
            throw std::out_of_range("Invalid match index");
        }
    }

    void displayMatchScorecard(int index) const {
        if (index >= 0 && index < matches.size()) {
            matches[index]->getScorecard();
        } else {
            throw std::out_of_range("Invalid match index");
        }
    }
};

int main() {
    try {
        // Create teams
        std::shared_ptr<BattingTeam> india = std::make_shared<BattingTeam>("India", "India", "Rahul Dravid");
        std::shared_ptr<BowlingTeam> australia = std::make_shared<BowlingTeam>("Australia", "Australia", "Andrew McDonald");

        // Add players (example)
        india->addPlayer(std::make_shared<Player>(1, "Rohit Sharma", "36", "Male", Role::BATSMAN));
        india->addPlayer(std::make_shared<Player>(2, "Virat Kohli", "35", "Male", Role::BATSMAN));
        australia->addPlayer(std::make_shared<Player>(3, "Pat Cummins", "30", "Male", Role::BOWLER));
        australia->addPlayer(std::make_shared<Player>(4, "Mitchell Starc", "33", "Male", Role::BOWLER));

        // Create a match
        std::shared_ptr<Match> match1 = std::make_shared<Match>(india, australia, 50);

        // Update scores and wickets
        match1->updateScore(1, 50);
        match1->updateWicket(1);
        match1->updateScore(2, 25);

        match1->getScorecard();

        Cricbuzz cricbuzz;
        cricbuzz.addMatch(match1);

        cricbuzz.getMatches();

        cricbuzz.displayMatchScorecard(0);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
