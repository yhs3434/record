package factory;

public class NYPizzaStore extends PizzaStore {
	
	@Override
	protected Pizza createPizza(String type) {
		// TODO Auto-generated method stub
		Pizza pizza = null;
		if (type.equals("cheese")) {
			pizza = new CheesePizza(new NYPizzaIngredientFactory());
		} else if (type.equals("clam")) {
			pizza = new ClamPizza(new NYPizzaIngredientFactory());
		} 
		return pizza;
	}

}
