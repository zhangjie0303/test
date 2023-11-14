public class HelloWorldJNI {
    static {
        System.loadLibrary("hello"); // libhello.so
    }
    
    public static void main(String[] args) {
        new HelloWorldJNI().sayHello();
		long sum = new HelloWorldJNI().sumIntegers(10, 20);
		System.out.println("sum:" + sum);
        String str = new HelloWorldJNI().itoa(88888888);
		System.out.println("str:" + str);

        // cpp call java
		HelloWorldJNI instance = new HelloWorldJNI();
		UserData newUser = instance.createUser("Zhangjie");
		instance.printUserName(newUser);
    }
 
	// java call cpp
    // 定义原生sayHello()方法
    private native void sayHello();

    // 定义原生sumIntegers方法，返回值是long，有两个int入参
    private native long sumIntegers(int first, int second);

    private native String itoa(long num);

	// cpp call java
	// 定义原生createUser方法
    public native UserData createUser(String name);
    //定义原生printUserName方法
    public native String printUserName(UserData user);
}
