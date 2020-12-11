package decorator;

class HouseBlend extends Beverage {
	public HouseBlend() {
		description = "하우스 블렌드 커피";
	}
	@Override
	public double cost() {
		// TODO Auto-generated method stub
		return .89;
	}

}
