package facade;

public class HomeTheaterTestDrive {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		HomeTheaterFacade homeTheater = new HomeTheaterFacade(amp, tuner, dvd,cd,projector,screen,lights,popper);
		homeTheater.watchMovie("Lion king");
		homeTheater.endMovie();
	}

}
