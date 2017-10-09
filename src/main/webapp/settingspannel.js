class SettingsPannel extends React.Component {

    render(){
        return <div className="panel panel-default">
            <div className="panel-heading">
                <a data-toggle="collapse" href="#collapsesettings" className="panel-title">
                Settings
                </a>
            </div>
            <div id="collapsesettings" className="panel-collapse collapse in">
                <div className="panel-body">
                    <form>
                        <div className="form-group">
                            <label>Url</label>
                            <input type="text" className="form-control" ref="jorgulr" placeholder="Jorg Api URL"/>
                        </div>
                        <div className="form-group">
                            <label>Email address</label>
                            <input type="text" className="form-control" ref="jorgusr" placeholder="Username"/>
                        </div>
                        <div className="form-group">
                            <label>Password</label>
                            <input type="text" className="form-control" ref="jorgpass" placeholder="Password"/>
                        </div>

                        <button type="button" onClick={this.onSubmitClick.bind(this)} className="btn btn-default">Store</button>
                    </form>
                </div>
            </div>
        </div>;
    }

    componentDidMount() {
        let _this = this;
        axios.get("/jorgapi")
            .then(function(result) {
                this.refs.jorgusr.value = result.data.username || "unset";
                this.refs.jorgpass.value = result.data.password || "unset";
                this.refs.jorgulr.value = result.data.baseurl || "unset";
            }.bind(this));
    }

    onSubmitClick() {
        axios.put("/jorgapi", {
            username: this.refs.jorgusr.value,
            password: this.refs.jorgpass.value,
            baseurl: this.refs.jorgulr.value
        })
        .then(function(result){
            alert("Setting stored");
        }).catch(function(error) {
            alert("Could not update credentials; " + error)
        });
    }
}