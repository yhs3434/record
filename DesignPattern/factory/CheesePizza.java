package factory;

public class CheesePizza extends Pizza {
	PizzaIngredientFactory ingredientFactory;
	
	public CheesePizza (PizzaIngredientFactory ingredientFactory) {
		this.ingredientFactory = ingredientFactory;
	}
	
	@Override
	void prepare() {
		// TODO Auto-generated method stub
		System.out.println("Preparing " + name);
		dough = ingredientFactory.createDough();
		sauce = ingredientFactory.createSauce();
		cheese = ingredientFactory.createCheese();
	}

}
