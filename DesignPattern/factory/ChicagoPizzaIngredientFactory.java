package factory;

public class ChicagoPizzaIngredientFactory implements PizzaIngredientFactory {
	@Override
	public Dough createDough() {
		// TODO Auto-generated method stub
		return new Dough("NY");
	}

	@Override
	public Sauce createSauce() {
		// TODO Auto-generated method stub
		return new Sauce("NY");
	}

	@Override
	public Cheese createCheese() {
		// TODO Auto-generated method stub
		return new Cheese("NY");
	}

	@Override
	public Veggies createVeggies() {
		// TODO Auto-generated method stub
		return new Veggies("NY");
	}

	@Override
	public Pepperoni createPepperoni() {
		// TODO Auto-generated method stub
		return new Pepperoni("NY");
	}

	@Override
	public Clams createClam() {
		// TODO Auto-generated method stub
		return new Clams("NY");
	}
}
