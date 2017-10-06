//this is a customized version of a part of the react-bootstrap-slider library by brownieboy
//https://github.com/brownieboy/react-bootstrap-slider

class ReactBootstrapSlider extends React.Component {
    constructor(props) {
        super(props);
    }

    componentDidMount() {
        this.mySlider = new Slider(this.node, this.props);

        if (this.props.slideStop) {
            this.mySlider.on("slideStop", this.props.slideStop);
        }
    }

    componentDidUpdate() {
        this.mySlider.setValue(this.props.value);
    }

    componentWillUnmount() {
        this.mySlider.destroy();
    }

    render() {
        return <div ref={node => this.node = node} />;
    }
}