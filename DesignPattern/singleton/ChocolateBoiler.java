package singleton;

public class ChocolateBoiler {
	private volatile static ChocolateBoiler chocolateBoiler;
	
	private boolean empty;
	private boolean boiled;
	
	private ChocolateBoiler() {
		empty = true;
		boiled = false;
	}
	
	public static ChocolateBoiler getInstance() {
		if(chocolateBoiler == null) {
			synchronized (ChocolateBoiler.class) {
				chocolateBoiler = new ChocolateBoiler();
			}
		}
		return chocolateBoiler;
	}
	
	public void fill() {
		if(isEmpty()) {
			empty = false;
			boiled = false;
		}
	}
	
	public void drain() {
		if (!isEmpty() && isBoiled()) {
			empty = true;
		}
	}
	
	public void boil() {
		if(!isEmpty() && !isBoiled()) {
			boiled = true;
		}
	}
	
	private boolean isEmpty() {
		return empty;
	}
	
	private boolean isBoiled() {
		return boiled;
	}
}
