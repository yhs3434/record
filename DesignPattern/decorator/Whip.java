package decorator;

class Whip extends CondimentDecorator {
	Beverage beverage;
	
	public Whip(Beverage b) {
		beverage = b;
	}
	
	@Override
	public String getDescription() {
		// TODO Auto-generated method stub
		return beverage.getDescription() + ", »€";
	}

	@Override
	public double cost() {
		// TODO Auto-generated method stub
		return beverage.cost() + .15;
	}

}
