class Console {
    public static void main(String[] args) {
    	System.out.println("Hello, world!");
    	
        if (args.length == 1) {
            System.out.println("Input taken: " + args[0]);
        } else if (args.length == 2) {
            try {
                for (int i = 0; i < Integer.parseInt(args[0]); i++) {
                    System.out.println(args[1]);
                }
            } catch (NumberFormatException e) {
            	System.out.println("First argument must be int");
            }
        }
    }
}
