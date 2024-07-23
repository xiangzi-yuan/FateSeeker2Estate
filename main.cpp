//问题描述
/*
杂货铺 Lv.10 279
租金收益
每相邻一间不同的房子，收益 + 60 % 。

民宅 Lv.10 310
租金收益
收租频率上升 + 6 % 。

药水铺 Lv.10 248
租金收益
相邻的房子, 功能性 + 50 %, 自己会受其他药铺影响。

武器铺 Lv.10 124
租金收益
战斗获得的体悟 + 10 % 。

防具铺 Lv.10 租金收益  186
强化防具时，需求素材 - 8 % 。

饰品铺 Lv.10 粗金收益 450
收益相当高的店铺。

工匠铺 Ly.10 租金收益155
房子升级、改建时的所需金钱 - 20 % ，最大累计到 100 % 。

农舍 Lv.10 310
相邻的房子，收益提升 + 30 % 。

茶水馆 Lv.10 279
每相邻一间民宅，收益 + 40 % 。
* * *
* * *
* * *

*处为建筑位置,要求收益最高,该如何摆放

*/
/*
排除 民宅 药水铺 武器铺 防具铺 工匠铺
杂货铺 饰品铺 农舍 茶水馆
*/

#include<iostream>
#include <string>
#include <vector>

class Architecture {
protected:
    int ArchitectureType;
    float income;
    float addIncome;

public:
    Architecture() : ArchitectureType(0), income(0.0f), addIncome(0.0f) {}
    Architecture(int typ, float inc, float addinc) : ArchitectureType(typ), income(inc), addIncome(addinc) {}
    
    virtual ~Architecture() = default;

    int getType() const { return ArchitectureType; }
    float getTotalIncome() const { return income + addIncome; }
    void setIncome(float inc) { income = inc; }
    void setAddIncome(float addinc) { addIncome = addinc; }
    // 添加虚函数供子类重写
    virtual void CalculateAddIncome(const std::vector<std::vector<Architecture*>>& grid, int row, int col) = 0;
};

class Grocery : public Architecture  // 每相邻一间不同的房子，收益 + 60 % 。
{
public:
    Grocery() : Architecture(0, 279.0f, 0.0f) {}

    void CalculateAddIncome(const std::vector<std::vector<Architecture*>>& grid, int row, int col) override {
        float calculatedAddIncome = 0.0f;

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;

                int newRow = row + dr;
                int newCol = col + dc;

                if (newRow >= 0 && newRow < grid.size() && newCol >= 0 && newCol < grid[0].size()) {
                    if (grid[newRow][newCol]->getType() != this->getType()) {
                        calculatedAddIncome += this->income * 0.60f;
                    }
                }
            }
        }

        this->setAddIncome(calculatedAddIncome);
    }
};

class JewelryShop : public Architecture {
public:
    JewelryShop() : Architecture(1, 450.0f, 0.0f) {}

    void CalculateAddIncome(const std::vector<std::vector<Architecture*>>& grid, int row, int col) override {
        // JewelryShop 没有特殊的收益逻辑
        this->setAddIncome(0.0f);
    }
};

class Farmhouse : public Architecture {
public:
    Farmhouse() : Architecture(2, 310.0f, 0.0f) {}

    void CalculateAddIncome(const std::vector<std::vector<Architecture*>>& grid, int row, int col) override {
        // Farmhouse 没有特殊的收益逻辑
        this->setAddIncome(0.0f);
    }
};

//class Pantry : public Architecture // 每相邻一间民宅，收益 + 40 % 。
//{
//public:
//	Pantry() :Architecture(3, 279.0f, 0.0f) {}
//
//};
enum BuildingType { GROCERY, JEWELRYSHOP, FARMHOUSE };

// 网格大小
const int GRID_SIZE = 3;

// 计算网格的总收益
float calculateTotalIncome(const std::vector<std::vector<Architecture*>>& grid, const std::vector<Architecture*>& buildings) {
    float totalIncome = 0.0f;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            totalIncome += grid[i][j]->getTotalIncome();
        }
    }
    return totalIncome;
}


// 打印网格
void printGrid(const std::vector<std::vector<BuildingType>>& grid) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void backtrack(std::vector<std::vector<Architecture*>>& grid, int pos, std::vector<Architecture*>& buildings, float& maxIncome, std::vector<std::vector<Architecture*>>& bestGrid) {
    if (pos == GRID_SIZE * GRID_SIZE) {
        float currentIncome = calculateTotalIncome(grid, buildings);
        if (currentIncome > maxIncome) {
            maxIncome = currentIncome;
            bestGrid = grid;
        }
        return;
    }

    int row = pos / GRID_SIZE;
    int col = pos % GRID_SIZE;

    for (int type = GROCERY; type <= FARMHOUSE; type++) {
        grid[row][col] = buildings[type];

        // 计算并更新额外收益
        grid[row][col]->CalculateAddIncome(grid, row, col);

        backtrack(grid, pos + 1, buildings, maxIncome, bestGrid);
    }
}

int main() {
    // 创建建筑物对象
    Grocery grocery;
    JewelryShop jewelryShop;
    Farmhouse farmhouse;

    std::vector<Architecture*> buildings = { &grocery, &jewelryShop, &farmhouse };

    // 初始化网格（类型为 BuildingType）
    std::vector<std::vector<BuildingType>> grid(GRID_SIZE, std::vector<BuildingType>(GRID_SIZE, GROCERY));
    std::vector<std::vector<BuildingType>> bestGrid(GRID_SIZE, std::vector<BuildingType>(GRID_SIZE, GROCERY));

    // 将 BuildingType 网格转换为 Architecture* 网格
    std::vector<std::vector<Architecture*>> architectureGrid(GRID_SIZE, std::vector<Architecture*>(GRID_SIZE, nullptr));
    std::vector<std::vector<Architecture*>> bestArchitectureGrid(GRID_SIZE, std::vector<Architecture*>(GRID_SIZE, nullptr));

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            architectureGrid[i][j] = buildings[grid[i][j]];
        }
    }

    float maxIncome = 0.0f;

    // 执行回溯
    backtrack(architectureGrid, 0, buildings, maxIncome, bestArchitectureGrid);

    // 打印结果
    std::cout << "Maximum Income: " << maxIncome << std::endl;
    std::cout << "Best Grid Layout:" << std::endl;
    printGrid(bestGrid);

    return 0;
}
