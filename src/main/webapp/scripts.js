class Orphanidge extends React.Component {

    constructor(props) {
        super(props);
        this.state = { children: []};
    }

    componentDidMount() {
        let _this = this;
        this.serverRequest =
            axios.get("/child")
                .then(function(result) {
                    _this.setState({
                        children: result.data
                    });
                })
    }

    componentWillUnmount() {
        this.serverRequest.abort();
    }

    render(){
        return <div>
            <SettingsPannel/>
            {this.state.children.map(function(child, i){
                return <ChildPanel id={child.id} ip={child.ip} name={child.name} key={i}/>;
            })}
        </div>
    }
}

ReactDOM.render(<Orphanidge/>, document.getElementById('reactroot'));

