import java.util.Scanner;

public class Shell {
    private Scanner scanner;

    Shell() {
        scanner = new Scanner(System.in);
    }

    void listen() {
        while (true) {
            String nextLine = scanner.nextLine();
            String[] command = nextLine.split(" ");
            if (!recognizeCommand(command)) {   //FIXME: add quit state to check
                break;
            }
        }
    }

    private boolean recognizeCommand(String[] command) {
        switch (command[0]) {
            case "cd":  //cd <name>     : create a new file
                create(command);
                break;
            case "de":  //de <name>     : destroy named file
                destroy(command);
                break;
            case "op":  //op <name>     :open the named file for reading and writing
                open(command);
                break;
            case "cl":  //cl <index>    :close specified file
                close(command);
                break;
            case "rd":  //rd <index> <count>    :sequentially read a number of bytes <count> from the specified file <index> and display them on the terminal
                read(command);
                break;
            case "wr":  //wr <index> <char> <count> :sequentially write <count> number of <char>s into the specified file <index> at its current position
                write(command);
                break;
            case "sk":  //sk <index> <char> <count> : seek: set the current position of the specified file <index> to <pos>
                seek(command);
                break;
            case "dr":  //dr                        :directory: list the names of all files and their lengths
                directory(command);
                break;
            case "in":  //in <no_cyl> <no_surf> <no_sect> <sect_len> <disk_cont>    create a disk using the given dimension parameters and initialize it using the file <disk_cont> (copy of disk)
                createDisk(command);
                break;
            case "sv":  //sv <disk_cont>    :close all files and save the contents of the disk in the file <disk_cont>
                save(command);
                break;
            case "quit":
                return false;
            default:    //error
                error("No such command specified.");
                break;
        }
        return true;
    }

    private void create(String[] command) {
        if (command.length != 2) {
            error("Wrong command signature");
            return;
        }

        //TODO: create file
        System.out.println("File " + command[1] + " created.");
    }

    private void destroy(String[] command) {
        if (command.length != 2) {
            error("Wrong command signature");
            return;
        }

        //TODO: destroy file
        System.out.println("File " + command[1] + " destroyed.");
    }

    private void open(String[] command) {
        if (command.length != 2) {
            error("Wrong command signature");
            return;
        }

        //TODO: open file
        System.out.println("File " + command[1] + " opened.");
    }

    private void close(String[] command) {
        if (command.length != 2) {
            error("Wrong command signature");
            return;
        }

        //TODO: close file
        System.out.println("File " + command[1] + " closed.");
    }

    private void read(String[] command) {
        if (command.length != 3) {
            error("Wrong command signature");
            return;
        }

        //TODO: read file
        System.out.println(command[2] + "bytes read: ");//+<read bytes>
    }

    private void write(String[] command) {
        if (command.length != 4) {
            error("Wrong command signature");
            return;
        }

        //TODO: write to  file
        System.out.println(command[2] + "bytes written. ");
    }

    private void seek(String[] command) {
        if (command.length != 2) {
            error("Wrong command signature");
            return;
        }

        //TODO: seek in file
        System.out.println("Current position is " + command[1]);
    }

    private void directory(String[] command) {
        if (command.length != 1) {
            error("Wrong command signature");
            return;
        }

        //TODO: list files names and length
        System.out.println("Files in directory: ");

    }

    private void createDisk(String[] command) {
        if (command.length != 6) {
            error("Wrong command signature");
            return;
        }

        //TODO:
        System.out.println("Disk restored.");
    }

    private void save(String[] command) {
        if (command.length != 2) {
            error("Wrong command signature");
            return;
        }

        //TODO: save
        System.out.println("Disk saved.");
    }

    //TODO: Exception class ????
    private void error(String message) {
        System.out.println("Error: " + message);
    }
}
