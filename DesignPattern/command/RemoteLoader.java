package command;

public class RemoteLoader {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		RemoteControl remoteControl = new RemoteControl();
		
		LightOnCommand lightOnCommand = new LightOnCommand(new Light());
		LightOffCommand lightOffCommand = new LightOffCommand(new Light());
		remoteControl.setCommand(0, lightOnCommand, lightOffCommand);
		Command[] partyOnCommands = {lightOnCommand};
		Command[] partyOffCommands = {lightOffCommand};
		MacroCommand macroOnCommand = new MacroCommand(partyOnCommands);
		MacroCommand macroOffCommand = new MacroCommand(partyOffCommands);
		remoteControl.setCommand(2, macroOnCommand, macroOffCommand);
		
		remoteControl.onButtonWasPushed(0);
		remoteControl.onButtonWasPushed(2);
	}

}
