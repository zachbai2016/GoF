#include <iostream>

using namespace std;

// 1. 迭代器接口
/*
模板声明：template <typename T>
这是 C++ 模板的语法，表明接下来的类是一个泛型类
typename T 声明了一个类型参数 T，允许这个类处理任意数据类型
使用时可以用具体类型（如Iterator<int>、Iterator<std::string>）实例化
*/
template <typename T>
/*
类定义：class Iterator
定义了一个名为Iterator的类，作为所有具体迭代器的基类
*/
class Iterator
{
public:

    virtual ~Iterator() = default;

    // virtual T *first() = 0;

    virtual T *next() = 0;

    // virtual T *curr() = 0;

    virtual bool isDone() = 0;
};


// 2. 聚合接口
template <typename T>
class Aggregate
{
public:

    virtual ~Aggregate() = default;

    virtual Iterator<T>* createIterator() = 0;    /* Aggregate 中负责定义 创建 迭代器的 接口  */

    virtual int getSize() = 0;

    virtual T& getItem(int index) = 0;

    virtual void addItem(const T& item) = 0;
};

// 3. 具体迭代器实现 接受泛型
template <typename T>
class AIterator: public Iterator<T>
{
  private:
    Aggregate<T> *aggregate;  // 具体迭代器中的集合
    int index;

  public:
    explicit AIterator(Aggregate<T> *agg):aggregate(agg),index(0)
    {}

    #if 0
    T* first() override
    {
      index = 0;
      return aggregate->getSize() > 0 ? &aggregate->getItem(index) : nullptr;
    }
    #endif

    T* next() override
    {
      if (!isDone())
      {
        return &aggregate->getItem(index++);
      }
      return nullptr;
    }

    #if 0
    T* curr() override
    {
      if (this->index < aggregate->getSize())
      {
        return &aggregate->getItem(index);
      }
      return nullptr;
    }
    #endif

    bool isDone() override
    {
      return index >= aggregate->getSize();
    }
};

// 4. 具体聚合实现 - 接受泛型
template <typename T>
class AAggregate:public Aggregate<T>
{
  private:
    T *items;
    int size;
    int capacity;

  public:
    explicit AAggregate(int cap=10):capacity(cap),size(0)
    {
      items = new T[cap];
    }

    ~AAggregate() override
    {
      delete[] items;
    }

    Iterator<T> *createIterator() override
    {
      return new AIterator<T>(this);  // this
    }

    int getSize() override
    {
      return this->size;
    }

    T& getItem(int index) override
    {
      return items[index];
    }

    void addItem(const T& item) override
    {
        if (size >= capacity) {
            // 简单的扩容逻辑
            capacity *= 2;
            T* newItems = new T[capacity];
            for (int i = 0; i < size; i++) {
                newItems[i] = items[i];
            }
            delete[] items;
            items = newItems;
            cout << "enhance capacity end " << endl;
        }
        items[size++] = item;
    }
};

void mode_1_iterator(void)
{
      /* 1. Iterator */ 
    cout << "**************** 1. Iterator *******************************************************************" << endl;
    AAggregate<string> *stringAggregate = new AAggregate<string>(5);

    stringAggregate->addItem("First");
    stringAggregate->addItem("Second");
    stringAggregate->addItem("Third");
    stringAggregate->addItem("Fourth");
    stringAggregate->addItem("Fivth");

    Iterator<string> *iterator = stringAggregate->createIterator();
    while (!iterator->isDone())
    {
      cout << *iterator->next() << endl;
    }

    stringAggregate->addItem("Sixth");
    while (!iterator->isDone())
    {
      cout << *iterator->next() << endl;
    }

    delete iterator;
    delete stringAggregate;

}

int main()
{
    cout << "**************** Design Patterns: Elements of Reusable Object-Oriented Software ****************" << endl;
    mode_1_iterator();
}