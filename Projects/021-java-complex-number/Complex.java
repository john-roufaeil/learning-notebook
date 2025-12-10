public class Complex<T extends Number> {
    private T real;
    private T img;

    public Complex(T real, T img) {
        this.real = real;
        this.img = img;
    }

    public T getReal() { return real; }

    public T getImg() { return img; }

    public void setReal(T real) { this.real = real; }

    public void setImg(T img) { this.img = img; }

    public void print() {
        Boolean isNegImg = getImg().doubleValue() < 0;
        Boolean isZeroReal = getReal().doubleValue() == 0;
        Boolean isZeroImg = getImg().doubleValue() == 0;
        Boolean isOneImg = getImg().doubleValue() == 1;
        if (isZeroReal && isZeroImg) System.out.println("0");
        else {
            System.out.println(
                (isZeroReal? "" : getReal()) + 
                (isNegImg? "" : (isZeroImg || isZeroReal ? "" : "+")) +
                (isZeroImg? "" : (isOneImg ? (isNegImg ?  "-i" : "i") : getImg() + "i"))
            );
        }
    }

    public Complex<Double> add(Complex<? extends Number> other) {
        double newReal = getReal().doubleValue() + other.getReal().doubleValue();
        double newImg = getImg().doubleValue() + other.getImg().doubleValue();
        return new Complex<>(newReal, newImg);
    }
    
    public Complex<Double> sub(Complex<? extends Number> other) {
        double newReal = getReal().doubleValue() - other.getReal().doubleValue();
        double newImg = getImg().doubleValue() - other.getImg().doubleValue();
        return new Complex<>(newReal, newImg);
    }
}