//��������
/*
�ӻ��� Lv.10 279
�������
ÿ����һ�䲻ͬ�ķ��ӣ����� + 60 % ��

��լ Lv.10 310
�������
����Ƶ������ + 6 % ��

ҩˮ�� Lv.10 248
�������
���ڵķ���, ������ + 50 %, �Լ���������ҩ��Ӱ�졣

������ Lv.10 124
�������
ս����õ����� + 10 % ��

������ Lv.10 �������  186
ǿ������ʱ�������ز� - 8 % ��

��Ʒ�� Lv.10 �ֽ����� 450
�����൱�ߵĵ��̡�

������ Ly.10 �������155
�����������Ľ�ʱ�������Ǯ - 20 % ������ۼƵ� 100 % ��

ũ�� Lv.10 310
���ڵķ��ӣ��������� + 30 % ��

��ˮ�� Lv.10 279
ÿ����һ����լ������ + 40 % ��
* * *
* * *
* * *

*��Ϊ����λ��,Ҫ���������,����ΰڷ�

*/
/*
�ų� ��լ ҩˮ�� ������ ������ ������
�ӻ��� ��Ʒ�� ũ�� ��ˮ��
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
    // ����麯����������д
    virtual void CalculateAddIncome(const std::vector<std::vector<Architecture*>>& grid, int row, int col) = 0;
};

class Grocery : public Architecture  // ÿ����һ�䲻ͬ�ķ��ӣ����� + 60 % ��
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
        // JewelryShop û������������߼�
        this->setAddIncome(0.0f);
    }
};

class Farmhouse : public Architecture {
public:
    Farmhouse() : Architecture(2, 310.0f, 0.0f) {}

    void CalculateAddIncome(const std::vector<std::vector<Architecture*>>& grid, int row, int col) override {
        // Farmhouse û������������߼�
        this->setAddIncome(0.0f);
    }
};

//class Pantry : public Architecture // ÿ����һ����լ������ + 40 % ��
//{
//public:
//	Pantry() :Architecture(3, 279.0f, 0.0f) {}
//
//};
enum BuildingType { GROCERY, JEWELRYSHOP, FARMHOUSE };

// �����С
const int GRID_SIZE = 3;

// ���������������
float calculateTotalIncome(const std::vector<std::vector<Architecture*>>& grid, const std::vector<Architecture*>& buildings) {
    float totalIncome = 0.0f;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            totalIncome += grid[i][j]->getTotalIncome();
        }
    }
    return totalIncome;
}


// ��ӡ����
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

        // ���㲢���¶�������
        grid[row][col]->CalculateAddIncome(grid, row, col);

        backtrack(grid, pos + 1, buildings, maxIncome, bestGrid);
    }
}

int main() {
    // �������������
    Grocery grocery;
    JewelryShop jewelryShop;
    Farmhouse farmhouse;

    std::vector<Architecture*> buildings = { &grocery, &jewelryShop, &farmhouse };

    // ��ʼ����������Ϊ BuildingType��
    std::vector<std::vector<BuildingType>> grid(GRID_SIZE, std::vector<BuildingType>(GRID_SIZE, GROCERY));
    std::vector<std::vector<BuildingType>> bestGrid(GRID_SIZE, std::vector<BuildingType>(GRID_SIZE, GROCERY));

    // �� BuildingType ����ת��Ϊ Architecture* ����
    std::vector<std::vector<Architecture*>> architectureGrid(GRID_SIZE, std::vector<Architecture*>(GRID_SIZE, nullptr));
    std::vector<std::vector<Architecture*>> bestArchitectureGrid(GRID_SIZE, std::vector<Architecture*>(GRID_SIZE, nullptr));

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            architectureGrid[i][j] = buildings[grid[i][j]];
        }
    }

    float maxIncome = 0.0f;

    // ִ�л���
    backtrack(architectureGrid, 0, buildings, maxIncome, bestArchitectureGrid);

    // ��ӡ���
    std::cout << "Maximum Income: " << maxIncome << std::endl;
    std::cout << "Best Grid Layout:" << std::endl;
    printGrid(bestGrid);

    return 0;
}
