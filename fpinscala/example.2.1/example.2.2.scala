
object MyFib
{
    def fib(n:Int):Int = {
        def go(n:Int, b:Int, a:Int):Int = {
            if ( n == 0 ) a
            else
                go(n-1, a, b+a)
        }
        go(n, 0, 1)
    }

    def main(args:Array[String]):Unit={
        println(fib(5))
    }

}
