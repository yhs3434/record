package factory;

public class ChicagoPizzaStore extends PizzaStore {

	@Override
	protected Pizza createPizza(String type) {
		// TODO Auto-generated method stub
		Pizza pizza = null;
		if (type.equals("cheese")) {
			pizza = new CheesePizza(new ChicagoPizzaIngredientFactory());
		} else if (type.equals("clam")) {
			pizza = new ClamPizza(new ChicagoPizzaIngredientFactory());
		} 
		return pizza;
	}

}
