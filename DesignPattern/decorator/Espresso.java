package decorator;

class Espresso extends Beverage {
	public Espresso() {
		description = "에스프레소";
	}
	@Override
	public double cost() {
		// TODO Auto-generated method stub
		return 1.99;
	}

}
