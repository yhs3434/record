package decorator;

class Mocha extends CondimentDecorator {
	Beverage beverage;
	
	public Mocha(Beverage beverage) {
		this.beverage = beverage;
	}
	
	@Override
	public String getDescription() {
		// TODO Auto-generated method stub
		return this.beverage.getDescription() + ", ¸ğÄ«";
	}

	@Override
	public double cost() {
		// TODO Auto-generated method stub
		return this.beverage.cost() + .20;
	}

}
