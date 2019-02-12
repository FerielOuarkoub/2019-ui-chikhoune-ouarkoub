type user = {
  email: string,
  password: string,
  name: string,
};

let initUser = {email: "", password: "",name: ""};

let updateEmail: (user, string) => user =
  (user, newEmail) => {
    email: newEmail,
    password: user.password,
    name: user.name,
  };

let updatePassword: (user, string) => user =
  (user, newPassword) => {
    email: user.email,
    password: newPassword,
    name: user.name,
  };

let encodeToJson: user => string =
  user =>
    Json.Encode.(
      object_([
        ("email", user##email),
        ("password", user##password),
        ("name", user##name),
      ])
    );
let encodeToJson: (string, string) => string =
  (email, pass) =>
    Json.Encode.(
      object_([("email", user##email), ("password", user##password)])
    );
