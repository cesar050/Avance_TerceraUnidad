class Producto {
    constructor(nombre, precio) {
        this.nombre = nombre;
        this.precio = precio;
    }

    formatearProducto() {
        return `El producto ${this.nombre} tiene un precio de: ${this.precio}`;
    }
}

const producto1 = new Producto('Camisa', 20);
console.log(producto1.formatearProducto());

class slider {
    constructor(){
        this.nombre = document.getElementById('nombre');
        this.value = document.getElementById('value');
    }


}
const slider1 = new slider();

console.log(slider1);