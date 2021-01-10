package main

import "fmt"

// var x string = "Hello world"

func main() {
	/*
		var x string = "Hello World"
		fmt.Println(x)
	*/

	/*
		x := "Hello World"
		fmt.Println(x)
	*/

	/*
		fmt.Println(x)
		f()
	*/

	fmt.Print("Enter a number: ")
	var input float64
	fmt.Scanf("%f", &input)

	output := input * 2
	fmt.Println(output)
}

/*
func f() {
	fmt.Println(x)
}
*/
