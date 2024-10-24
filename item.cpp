class Item
{
public:
  // Atributes
  int value;
  int weight;

  // Methods
  Item(int value, int weight)
  {
    this->value = value;
    this->weight = weight;
  }

  int getValue() { return value; }

  int getWeight() { return weight; }
};