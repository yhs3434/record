package decorator;

class Decaf extends Beverage {
	public Decaf() {
		description = "디카페인 커피";
	}
	@Override
	public double cost() {
		// TODO Auto-generated method stub
		return 1.59;
	}

}
